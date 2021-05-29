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
    return new Mesh(vertices, indices, textures);
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
}