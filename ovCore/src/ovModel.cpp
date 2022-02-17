#include "ovModel.h"
#include <ovGraphicsAPI.h>
#include <ovQuaternion.h>

namespace ovEngineSDK {
  
  #define MIN_SPHERE_SECTOR 3
  #define MIN_SPHERE_STACK 2

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
      if (posSlash == String::npos) {
        if (posSlash > realPos) {
          posSlash = realPos;
        }
      }
    }
    if (realPos == 0) {
      return "/" + file;
    }

    return "/" + file.substr(realPos + 1, file.length() - realPos);  
  }

  Model::~Model() {
    m_meshes.clear();
    m_modelTextures.clear();
    delete m_modelScene;
  }

  void Model::update(float delta) {
    delta;
  }

  void
  Model::render() {
    for (auto& mesh : m_meshes) {
      mesh->draw(m_textureSampler);
    }
  }

  void
  Model::load(String const& path, bool notexture) {
    //Read file via assimp
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
                                             aiProcessPreset_TargetRealtime_MaxQuality |
                                             aiProcess_ConvertToLeftHanded |
                                             aiProcess_Triangulate);
    //Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      return;
    }
    m_modelScene = importer.GetOrphanedScene();
    memcpy(&m_globalTransform, &m_modelScene->mRootNode->mTransformation, sizeof(Matrix4));
    m_globalTransform = m_globalTransform.inverse();
    //Retrieve the directory path of the file
    m_directory = path.substr(0, path.find_last_of('/'));
    //Process assimp's root node recursively
    processNode(scene->mRootNode, scene, notexture);
    //Create texture sampler for model's textures
    m_textureSampler = g_graphicsAPI().createSamplerState(FILTER_LEVEL::FILTER_LINEAR,
                                                          FILTER_LEVEL::FILTER_LINEAR,
                                                          FILTER_LEVEL::FILTER_LINEAR,
                                                          false,
                                                          0,
                                                          WRAPPING::WRAP,
                                                          COMPARISON::NEVER);
  }

  void Model::transformBones(float delta, Vector<Matrix4>& Transforms) {
    int32 totalBones = 0;
    for (auto& tempMesh : m_meshes) {
      totalBones += tempMesh->m_numBones;
    }
    float timeInTicks = delta * 
                        static_cast<float>(m_modelScene->mAnimations[0]->mTicksPerSecond);
    float animTime = Math::fmod(
    timeInTicks, static_cast<float>(m_modelScene->mAnimations[0]->mDuration));

    for (auto& tempMesh : m_meshes) {
      //readNodeHierarchy(animTime, m_modelScene->mRootNode, Matrix4::IDENTITY, tempMesh);
    }
    Transforms.resize(totalBones);

    SIZE_T meshCount = m_meshes.size();
    for (SIZE_T i = 0; i < meshCount; ++i) {
      for (int32 j = 0; j < totalBones; ++j) {
          Transforms[j] = m_meshes[i]->m_boneInfo[j].FinalTransform;
      }
    }
  }

  void
  Model::getModelInfo(Vector<Vector3>& vertices,
                      Vector<Vector2I>& indices,
                      Vector<Vector3>& normals,
                      Vector<Vector2>& uvs) {
    
  }

  int32
  Model::getMeshCount() {
    return m_meshes.empty() ? 0 : static_cast<int32>(m_meshes.size());
  }

  void
  Model::addMesh(const Vector<MeshVertex> vertices,
                 const Vector<uint32> indices,
                 const Vector<MeshTexture> textures) {
    m_meshes.push_back(make_shared<Mesh>(vertices, indices, textures, nullptr));
  }

  SPtr<Model> Model::createBox() {
    SPtr<Model> Box = make_shared<Model>();
    Vector<MeshVertex> boxVertices;
    Vector<uint32> boxIndices;
    Vector<MeshTexture> boxTextures;
    MeshVertex v;

    v.Normal = Vector3(1.f, 1.f, 1.f);
    v.Tangent = Vector3(1.f, 1.f, 1.f);
    v.Bitangent = Vector3(1.f, 1.f, 1.f);

    v.Position = Vector3(-1.f, 1.f, -1.f); v.TexCoords = Vector2(0.f, 0.f);    
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, 1.f, -1.f); v.TexCoords = Vector2(1.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, 1.f, 1.f); v.TexCoords = Vector2(1.f, 1.f);
    boxVertices.push_back(v);
    v.Position = Vector3(-1.f, 1.f, 1.f); v.TexCoords = Vector2(0.f, 1.f);
    boxVertices.push_back(v);

    v.Position = Vector3(-1.f, -1.f, -1.f); v.TexCoords = Vector2(0.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, -1.f, -1.f); v.TexCoords = Vector2(1.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, -1.f, 1.f); v.TexCoords = Vector2(1.f, 1.f);
    boxVertices.push_back(v);
    v.Position = Vector3(-1.f, -1.f, 1.f); v.TexCoords = Vector2(0.f, 1.f);
    boxVertices.push_back(v);

    v.Position = Vector3(-1.f, -1.f, 1.f); v.TexCoords = Vector2(0.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(-1.f, -1.f, -1.f); v.TexCoords = Vector2(1.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(-1.f, 1.f, -1.f); v.TexCoords = Vector2(1.f, 1.f);
    boxVertices.push_back(v);
    v.Position = Vector3(-1.f, 1.f, 1.f); v.TexCoords = Vector2(0.f, 1.f);
    boxVertices.push_back(v);

    v.Position = Vector3(1.f, -1.f, 1.f); v.TexCoords = Vector2(0.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, -1.f, -1.f); v.TexCoords = Vector2(1.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, 1.f, -1.f); v.TexCoords = Vector2(1.f, 1.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, 1.f, 1.f); v.TexCoords = Vector2(0.f, 1.f);
    boxVertices.push_back(v);

    v.Position = Vector3(-1.f, -1.f, -1.f); v.TexCoords = Vector2(0.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, -1.f, 1.f); v.TexCoords = Vector2(1.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, 1.f, -1.f); v.TexCoords = Vector2(1.f, 1.f);
    boxVertices.push_back(v);
    v.Position = Vector3(-1.f, 1.f, -1.f); v.TexCoords = Vector2(0.f, 1.f);
    boxVertices.push_back(v);

    v.Position = Vector3(-1.f, -1.f, 1.f); v.TexCoords = Vector2(0.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, -1.f, 1.f); v.TexCoords = Vector2(1.f, 0.f);
    boxVertices.push_back(v);
    v.Position = Vector3(1.f, 1.f, 1.f); v.TexCoords = Vector2(1.f, 1.f);
    boxVertices.push_back(v);
    v.Position = Vector3(-1.f, 1.f, 1.f); v.TexCoords = Vector2(0.f, 1.f);
    boxVertices.push_back(v);

    boxIndices.push_back(3); boxIndices.push_back(1); boxIndices.push_back(0);

    boxIndices.push_back(2); boxIndices.push_back(1); boxIndices.push_back(3);

    boxIndices.push_back(6); boxIndices.push_back(4); boxIndices.push_back(5);

    boxIndices.push_back(7); boxIndices.push_back(4); boxIndices.push_back(6);

    boxIndices.push_back(11); boxIndices.push_back(9); boxIndices.push_back(8);

    boxIndices.push_back(10); boxIndices.push_back(9); boxIndices.push_back(11);

    boxIndices.push_back(14); boxIndices.push_back(12); boxIndices.push_back(13);

    boxIndices.push_back(15); boxIndices.push_back(12); boxIndices.push_back(14);

    boxIndices.push_back(19); boxIndices.push_back(17); boxIndices.push_back(16);

    boxIndices.push_back(18); boxIndices.push_back(17); boxIndices.push_back(19);

    boxIndices.push_back(22); boxIndices.push_back(20); boxIndices.push_back(21);

    boxIndices.push_back(23); boxIndices.push_back(20); boxIndices.push_back(22);

    MeshTexture mTexture;
    mTexture.TextureMesh = g_graphicsAPI().createTextureFromFile(
      "resources/textures/missingtexture.png");

    boxTextures.push_back(mTexture);
    boxTextures.push_back(mTexture);
    boxTextures.push_back(mTexture);
    boxTextures.push_back(mTexture);

    Box->m_textureSampler = g_graphicsAPI().createSamplerState(FILTER_LEVEL::FILTER_LINEAR,
                                                      FILTER_LEVEL::FILTER_LINEAR,
                                                      FILTER_LEVEL::FILTER_LINEAR,
                                                      false,
                                                      0,
                                                      WRAPPING::WRAP,
                                                      COMPARISON::NEVER);

    Box->addMesh(boxVertices, boxIndices, boxTextures);
    return Box;
  }

  SPtr<Model>
  Model::createSphere(float radius, uint32 sectors, uint32 stacks) {
    int32 sphereSectors = sectors < MIN_SPHERE_SECTOR ? MIN_SPHERE_SECTOR : sectors;
    int32 spehereStacks = stacks < MIN_SPHERE_STACK ? MIN_SPHERE_STACK : stacks;
    
    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;

    float sectorStep = 2 * Math::PI / sphereSectors;
    float stackStep = Math::PI / spehereStacks;
    float sectorAngle, stackAngle;

    Vector<MeshVertex> sphereVertices;
    Vector<uint32> sphereIndices;
    Vector<MeshTexture> sphereTextures;
    MeshVertex vertex;
    vertex.Tangent = Vector3(1.0f, 1.0f, 1.0f);
    vertex.Bitangent = Vector3(1.0f, 1.0f, 1.0f);

    //Vertices
    for (uint32 i = 0; i <= spehereStacks; ++i) {
      stackAngle = Math::PI / 2 - i * stackStep;
      xy = radius * Math::cos(stackAngle);
      z = radius * Math::sin(stackAngle);

      for (uint32 j = 0; j <= sphereSectors; ++j) {
        sectorAngle = j * sectorStep;

        //Vertex
        x = xy * Math::cos(sectorAngle);
        y = xy * Math::sin(sectorAngle);
        vertex.Position = Vector3(x, y, z);
        //Normal
        nx = x * lengthInv;
        ny = y * lengthInv;
        nz = z * lengthInv;
        vertex.Normal = Vector3(nx, ny, nz);
        //Texcoords
        s = static_cast<float>(j / sphereSectors);
        t = static_cast<float>(i / spehereStacks);
        vertex.TexCoords = Vector2(s, t);
        sphereVertices.push_back(vertex);
      }
    }
    //Indices
    uint32 k1, k2;
    for (uint32 i = 0; i < spehereStacks; ++i) {
      k1 = i * (sphereSectors + 1);
      k2 = k1 + sphereSectors + 1;

      for (int32 j = 0; j < sphereSectors; ++j, ++k1, ++k2) {
        if (i != 0) {
          sphereIndices.push_back(k1);
          sphereIndices.push_back(k2);
          sphereIndices.push_back(k1 + 1);
        }

        if (i != (spehereStacks - 1)) {
          sphereIndices.push_back(k1 + 1);
          sphereIndices.push_back(k2);
          sphereIndices.push_back(k2 + 1);
        }
      }

    }
    SPtr<Model> Sphere = make_shared<Model>();
    MeshTexture mTexture;
    mTexture.TextureMesh = g_graphicsAPI().createTextureFromFile(
      "resources/textures/missingtexture.png");

    sphereTextures.push_back(mTexture);
    sphereTextures.push_back(mTexture);
    sphereTextures.push_back(mTexture);
    sphereTextures.push_back(mTexture);
    Sphere->addMesh(sphereVertices, sphereIndices, sphereTextures);
    Sphere->m_textureSampler = g_graphicsAPI().createSamplerState(
                                FILTER_LEVEL::FILTER_LINEAR,
                                FILTER_LEVEL::FILTER_LINEAR,
                                FILTER_LEVEL::FILTER_LINEAR,
                                false,
                                0,
                                WRAPPING::WRAP,
                                COMPARISON::NEVER);
    return Sphere;
  }

  

  SPtr<Model> Model::createCylinder() {
    return SPtr<Model>();
  }

  void
  Model::processNode(aiNode* node, const aiScene* scene, bool texture) {
    //Process each mesh located at the current node
    for (uint32 i = 0; i < node->mNumMeshes; ++i) {
      aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
      m_meshes.push_back(processMesh(mesh, scene, texture));
    }
    for (uint32 i = 0; i < node->mNumChildren; ++i) {
      processNode(node->mChildren[i], scene, texture);
    }
  }

  SPtr<Mesh>
  Model::processMesh(aiMesh* mesh, const aiScene* scene, bool texture) {
    //Data to fill
    Vector<MeshVertex> vertices;
    Vector<uint32> indices;
    Vector<MeshTexture> textures;

    //Walk through each of the mesh's vertices
    for (uint32 i = 0; i < mesh->mNumVertices; ++i) {
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
      v.Bitangent.x = mesh->mBitangents[i].x;
      v.Bitangent.y = mesh->mBitangents[i].y;
      v.Bitangent.z = mesh->mBitangents[i].z;
      vertices.push_back(v);
    }
    //Go through each of the mesh's faces and retrieve the corresponding indices
    for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
      aiFace face= mesh->mFaces[i];
      //Retrieve all indices of the face and store them in the indices vector
      for (uint32 j = 0; j < face.mNumIndices; ++j) {
        indices.push_back(face.mIndices[j]);
      }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    Vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, texture);
    Vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, texture);
    Vector<MeshTexture> metallicMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, texture);
    Vector<MeshTexture> roughMaps = loadMaterialTextures(material, aiTextureType_SHININESS, texture);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    textures.insert(textures.end(), metallicMaps.begin(), metallicMaps.end());
    textures.insert(textures.end(), roughMaps.begin(), roughMaps.end());
    return make_shared<Mesh>(vertices, indices, textures, mesh);
  }

  Vector<MeshTexture>
  Model::loadMaterialTextures(aiMaterial* material, aiTextureType type, bool texture) {
    Vector<MeshTexture> textures;

    if (!texture) {
      for (uint32 i = 0; i < material->GetTextureCount(type); ++i) {
        aiString aiStr;
        material->GetTexture(type, i, &aiStr);
        String str = String(aiStr.C_Str());
        str = m_directory + getTexturePath(str);
        bool skip = false;
        for (auto& textureModel : m_modelTextures) {
          if (strcmp(textureModel.Path.data(), str.data()) == 0) {
            textures.push_back(textureModel);
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
    }
    else {
      MeshTexture mTexture;
      mTexture.TextureMesh = g_graphicsAPI().createTextureFromFile(
                              "resources/textures/missingtexture.png");
      mTexture.Path = "missingtexture.png";
      textures.push_back(mTexture);
      m_modelTextures.push_back(mTexture);
    }
    
    return textures;
  }
  const aiNodeAnim*
  Model::findAnimationNode(const aiAnimation* anim, const String node) {
    for (uint32 i = 0; i < anim->mNumChannels; ++i) {
      const aiNodeAnim* T = anim->mChannels[i];
      if (String(T->mNodeName.data) == node) {
        return T;
      }
    }
    return nullptr;
  }
  void
  Model::readNodeHierarchy(float animTime,
                           const aiNode* node,
                           const Matrix4 tParent,
                           Mesh* modelMesh) {
    String nodeName(node->mName.data);
    
    Matrix4 Transform;
    memcpy(&Transform, &node->mTransformation, sizeof(Matrix4));
    Transform = Transform.transpose();
    const aiNodeAnim* animNode = findAnimationNode(m_modelScene->mAnimations[0], nodeName);
    if (animNode) {
      //Interpolate scaling and generate scaling transformation matrix
      aiVector3D aiscaling;
      calcInterpolatedScale(aiscaling, animTime, animNode);
      Matrix4 scaleMat = Matrix4::IDENTITY;
      //Translate the values from aiVector3D to Vector3
      Vector3 scaling(aiscaling.x, aiscaling.y, aiscaling.z);
      scaleMat = Matrix4::scale(scaleMat, scaling);

      //Interpolate rotation and generate rotation transform matrix
      aiQuaternion aiquat;
      calcInterpolatedRot(aiquat, animTime, animNode);
      aiquat.Conjugate();
      Quaternion rotation(static_cast<float>(aiquat.x), static_cast<float>(aiquat.y),
                          static_cast<float>(aiquat.z), static_cast<float>(aiquat.w));
      Matrix4 rotMat = Matrix4::fromQuat(rotation);

      //Interpolate translation and generate translation transform matrix
      aiVector3D aitranslate;
      calcInterpolatedPos(aitranslate, animTime, animNode);
      Matrix4 transMat = Matrix4::IDENTITY;
      transMat.xVector.w = aitranslate.x;
      transMat.yVector.w = aitranslate.y;
      transMat.zVector.w = aitranslate.z;

      //Combine above transformations
      Transform = transMat * rotMat * scaleMat;
    }
    Matrix4 globalTransform = tParent * Transform;
    if (modelMesh->m_boneMapping.find(nodeName) != modelMesh->m_boneMapping.end()) {
      uint32 boneIndex = modelMesh->m_boneMapping[nodeName];
      modelMesh->m_boneInfo[boneIndex].FinalTransform =
                               m_globalTransform *
                               globalTransform *
                               modelMesh->m_boneInfo[boneIndex].BoneOffset;
    }
    for (uint32 i = 0; i < node->mNumChildren; ++i) {
      readNodeHierarchy(animTime, node->mChildren[i], globalTransform, modelMesh);
    }
  }
  uint32
  Model::findPosition(float animTime, const aiNodeAnim* nodeAnimation) {
    for (uint32 i = 0; i < nodeAnimation->mNumPositionKeys - 1; ++i) {
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
    for (uint32 i = 0; i < nodeAnimation->mNumRotationKeys - 1; ++i) {
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
    for (uint32 i = 0; i < nodeAnimation->mNumScalingKeys - 1; ++i) {
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
    float factor = (animTime -
                   static_cast<float>(nodeAnimation->mPositionKeys[posIndex].mTime)) / delta;
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

    if (nodeAnimation->mNumRotationKeys == 1) {
      Out = nodeAnimation->mRotationKeys[0].mValue;
      return;
    }
    uint32 rotIndex = findRotation(animTime, nodeAnimation);
    uint32 nextRotIndex = rotIndex + 1;
    assert(nextRotIndex < nodeAnimation->mNumRotationKeys);
    float delta = static_cast<float>(nodeAnimation->mRotationKeys[nextRotIndex].mTime -
                                     nodeAnimation->mRotationKeys[rotIndex].mTime);
    float factor = (animTime - static_cast<float>(nodeAnimation->mRotationKeys[rotIndex].mTime)) / delta;
    assert(factor >= 0.f && factor <= 1.f);
    const aiQuaternion& Start = nodeAnimation->mRotationKeys[rotIndex].mValue;
    const aiQuaternion& End = nodeAnimation->mRotationKeys[nextRotIndex].mValue;
    aiQuaternion::Interpolate(Out, Start, End, factor);
    Out.Normalize();
  }
  void
  Model::calcInterpolatedScale(aiVector3D& Out,
                               float animTime,
                               const aiNodeAnim* nodeAnimation) {
    if (nodeAnimation->mNumScalingKeys == 1) {
      Out = nodeAnimation->mScalingKeys[0].mValue;
      return;
    }
    uint32 scalIndex = findScaling(animTime, nodeAnimation);
    uint32 nextScalIndex = scalIndex + 1;
    assert(nextScalIndex < nodeAnimation->mNumScalingKeys);
    float delta = static_cast<float>(nodeAnimation->mScalingKeys[nextScalIndex].mTime - 
                                     nodeAnimation->mScalingKeys[scalIndex].mTime);
    float factor = (animTime - static_cast<float>(nodeAnimation->mScalingKeys[scalIndex].mTime)) / delta;
    assert(factor >= 0.f && factor <= 1.f);
    const aiVector3D& Start = nodeAnimation->mScalingKeys[scalIndex].mValue;
    const aiVector3D& End = nodeAnimation->mScalingKeys[nextScalIndex].mValue;
    aiVector3D dTime = End - Start;
    Out = Start + factor * dTime;
  }
}