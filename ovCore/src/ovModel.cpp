#include "ovModel.h"
#include <ovGraphicsAPI.h>

namespace ovEngineSDK {
  
  String getTexturePath(String file) {
    size_t realPos = 0;
    size_t posInvSlash = file.rfind('\\');
    size_t posSlash = file.rfind('/');

    if (posInvSlash == String::npos) {
      if (posSlash != String::npos) {
        realPos = posSlash;
      }
    }
    else {
      realPos = posInvSlash;
      if (!posSlash == String::npos) {
        if (posSlash > realPos) {
          posSlash = realPos;
        }
      }
    }
    return file.substr(realPos, file.length() - realPos);
  }

  Model::~Model() {
    for (auto& mesh : m_meshes) {
      delete mesh;
    }
    m_meshes.clear();
    m_modelTextures.clear();
  }

  void
  Model::draw() {
    for (auto& mesh : m_meshes) {
      mesh->draw(m_textureSampler);
    }
  }

  void
  Model::load(String const& path) {
    //Read file via assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
                                             aiProcessPreset_TargetRealtime_Fast |
                                             aiProcess_ConvertToLeftHanded |
                                             aiProcess_FindInstances |
                                             aiProcess_ValidateDataStructure |
                                             aiProcess_OptimizeMeshes |
                                             aiProcess_Debone);
    //Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      return;
    }
    //Retrieve the directory path of the file
    m_directory = path.substr(0, path.find_last_of('/'));
    //Process assimp's root node recursively
    processNode(scene->mRootNode, scene);
    //Create texture sampler for model's textures
    m_textureSampler = g_graphicsAPI().createSamplerState(FILTER_LEVEL::FILTER_LINEAR,
                                                          FILTER_LEVEL::FILTER_LINEAR,
                                                          FILTER_LEVEL::FILTER_LINEAR,
                                                          0,
                                                          WRAPPING::WRAP);
  }

  void Model::transformBones(float delta, Vector<Matrix4>& Transforms) {
    Matrix4 identity = Matrix4::IDENTITY;
    int32 totalBones = 0;
    for (uint32 i = 0; i < m_meshes.size(); i++) {
      totalBones += m_meshes[i]->m_numBones;
    }
    float ticksPerSec = m_modelAnimation->mTicksPerSecond != 0 ?
                        m_modelAnimation->mTicksPerSecond : 25.f;
    float timeInTicks = delta * ticksPerSec;
    float animTime = Math::fmod(timeInTicks, m_modelAnimation->mDuration);

    for (uint32 i = 0; i < m_meshes.size(); i++) {
      readNodeHierarchy(animTime, , Matrix4::IDENTITY, m_meshes[i]);
    }
    Transforms.resize(totalBones);
    for (uint32 i = 0; i < totalBones; i++) {
      for (uint32 j = 0; j < m_meshes.size(); j++) {
        for (uint32 k = 0; k < m_meshes[j]->m_boneInfo.size(); k++) {
          Transforms[i] = m_meshes[j]->m_boneInfo[k].FinalTransform;
          i++;
        }
      }
    }
  }

  void
  Model::processNode(aiNode* node, const aiScene* scene) {
    //Process each mesh located at the current node
    for (uint32 i = 0; i < node->mNumMeshes; i++) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      m_meshes.push_back(processMesh(mesh, scene));
    }
    for (uint32 i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh*
  Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    //Data to fill
    Vector<MeshVertex>* vertices = new Vector<MeshVertex>();
    Vector<uint32>* indices = new Vector<uint32>();
    Vector<MeshTexture> textures;

    //Walk through each of the mesh's vertices
    for (uint32 i = 0; i < mesh->mNumVertices; i++) {
      MeshVertex v;
      //Positions
      v.Position.x = mesh->mVertices[i].x;
      v.Position.y = mesh->mVertices[i].y;
      v.Position.z = mesh->mVertices[i].z;
      //Texcoords
      //Check if mesh contains texcoords
      if (mesh->mTextureCoords[0]) {
        v.TexCoords.x = mesh->mTextureCoords[0][i].x;
        v.TexCoords.y = mesh->mTextureCoords[0][i].y;
      }
      else {
        v.TexCoords = Vector2(0.0f, 0.0f);
      }
      //Normals
      v.Normal.x = mesh->mNormals[i].x;
      v.Normal.y = mesh->mNormals[i].y;
      v.Normal.z = mesh->mNormals[i].z;
      //Tangents
      v.Tangent.x = mesh->mTangents[i].x;
      v.Tangent.y = mesh->mTangents[i].y;
      v.Tangent.z = mesh->mTangents[i].z;
      //Bitangents
      v.Tangent.x = mesh->mBitangents[i].x;
      v.Tangent.y = mesh->mBitangents[i].y;
      v.Tangent.z = mesh->mBitangents[i].z;
      vertices->push_back(v);
    }
    //Go through each of the mesh's faces and retrieve the corresponding indices
    for (uint32 i = 0; i < mesh->mNumFaces; i++) {
      aiFace face= mesh->mFaces[i];
      //Retrieve all indices of the face and store them in the indices vector
      for (uint32 j = 0; j < face.mNumIndices; j++) {
        indices->push_back(face.mIndices[j]);
      }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    Vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    return new Mesh(vertices, indices, textures, mesh);
  }

  Vector<MeshTexture>
  Model::loadMaterialTextures(aiMaterial* material, aiTextureType type) {
    Vector<MeshTexture> textures;

    for (uint32 i = 0; i < material->GetTextureCount(type); i++) {
      aiString aiStr;
      material->GetTexture(type, i, &aiStr);
      String str = String(aiStr.C_Str());
      str = m_directory + getTexturePath(str);
      bool skip = false;
      for (uint32 j = 0; j < m_modelTextures.size(); j++) {
        if (strcmp(m_modelTextures[j].Path.data(), str.data()) == 0) {
          textures.push_back(m_modelTextures[j]);
          skip = true;
          break;
        }
      }
      if (!skip) {
        MeshTexture mTexture;
        mTexture.TextureMesh = g_graphicsAPI().createTextureFromFile(str);
        mTexture.Path = str;
        textures.push_back(mTexture);
        m_modelTextures.push_back(mTexture);
      }
    }
    return textures;
  }
  void
  Model::readNodeHierarchy(float animTime,
                           const aiNode* node,
                           const Matrix4 tParent,
                           Mesh* modelMesh) {
    String nodeName(node->mName.data);
    
    //Transpose
    Matrix4 Transform;
    Transform.xVector.x = node->mTransformation.a1;
    Transform.xVector.y = node->mTransformation.a2;
    Transform.xVector.z = node->mTransformation.a3;
    Transform.xVector.w = node->mTransformation.a4;

    Transform.yVector.x = node->mTransformation.b1;
    Transform.yVector.y = node->mTransformation.b2;
    Transform.yVector.z = node->mTransformation.b3;
    Transform.yVector.w = node->mTransformation.b4;

    Transform.zVector.x = node->mTransformation.c1;
    Transform.zVector.y = node->mTransformation.c2;
    Transform.zVector.z = node->mTransformation.c3;
    Transform.zVector.w = node->mTransformation.c4;

    Transform.wVector.x = node->mTransformation.d1;
    Transform.wVector.y = node->mTransformation.d2;
    Transform.wVector.z = node->mTransformation.d3;
    Transform.wVector.w = node->mTransformation.d4;

    Transform = Transform.transpose();

    const aiNodeAnim* animNode = findAnimationNode(m_modelAnimation, nodeName);
    if (animNode) {
      //Interpolate scaling and generate scaling transformation matrix

    }
    Matrix4 globalTransform = tParent * Transform;
    if (modelMesh->m_boneMapping.find(nodeName) != modelMesh->m_boneMapping.end()) {
      uint32 boneIndex = modelMesh->m_boneMapping[nodeName];
      modelMesh->m_boneInfo[boneIndex].FinalTransform =
                                globalTransform * modelMesh->m_boneInfo[boneIndex].BoneOffset;
    }
    for (uint32 i = 0; i < node->mNumChildren; i++) {
      readNodeHierarchy(animTime, node->mChildren[i], globalTransform, modelMesh);
    }
  }
  uint32
  Model::findPosition(float animTime, const aiNodeAnim* nodeAnimation) {
    for (uint32 i = 0; i < nodeAnimation->mNumPositionKeys - 1; i++) {
      if (animTime < static_cast<float>(nodeAnimation->mPositionKeys[i + 1].mTime)) {
        return i;
      }
    }
    assert(0);
    return 0;
  }
  uint32
  Model::findRotation(float animTime, const aiNodeAnim* nodeAnimation) {
    assert(nodeAnimation->mNumRotationKeys > 0);
    for (uint32 i = 0; i < nodeAnimation->mNumRotationKeys - 1; i++) {
      if (animTime < static_cast<float>(nodeAnimation->mRotationKeys[i + 1].mTime)) {
        return i;
      }
    }
    assert(0);
    return 0;
  }
  uint32
  Model::findScaling(float animTime, const aiNodeAnim* nodeAnimation) {
    assert(nodeAnimation->mNumScalingKeys > 0);
    for (uint32 i = 0; i < nodeAnimation->mNumScalingKeys - 1; i++) {
      if (animTime < static_cast<float>(nodeAnimation->mScalingKeys[i + 1].mTime)) {
        return i;
      }
    }
    assert(0);
    return 0;
  }
  void
  Model::calcInterpolatedPos(aiVector3D& Out,
                             float animTime,
                             const aiNodeAnim* nodeAnimation) {
    if (nodeAnimation->mNumPositionKeys == 1) {
      Out = nodeAnimation->mPositionKeys[0].mValue;
      return;
    }
    uint32 posIndex = findPosition(animTime, nodeAnimation);
    uint32 nextPosIndex = posIndex + 1;
    assert(nextPosIndex < nodeAnimation->mNumPositionKeys);
    float delta = static_cast<float>(nodeAnimation->mPositionKeys[nextPosIndex].mTime - 
                                     nodeAnimation->mPositionKeys[posIndex].mTime);
    float factor = animTime -
                   static_cast<float>(nodeAnimation->mPositionKeys[posIndex].mTime / delta);
    assert(factor >= 0.f && factor <= 1.f);
    const aiVector3D& Start = nodeAnimation->mPositionKeys[posIndex].mValue;
    const aiVector3D& End = nodeAnimation->mPositionKeys[nextPosIndex].mValue;
    aiVector3D dTime = End - Start;
    Out = Start + factor * dTime;
  }
  void
  Model::calcInterpolatedRot(aiQuaternion& Out,
                             float animTime,
                             const aiNodeAnim* nodeAnimation) {
    
  }
  void
  Model::calcInterpolatedScale(aiVector3D& Out,
                               float animTime,
                               const aiNodeAnim* nodeAnimation) {
    
  }
}