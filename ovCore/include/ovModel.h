#pragma once
#include <ovPrerequisitesCore.h>
#include <ovMesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ovEngineSDK {
  class OV_CORE_EXPORT Model
  {
   public:
    Model() = default;
    ~Model();

    void draw();
    void load(String const& path);

   private:
    Vector<Mesh*> m_meshes;
    Vector<MeshTexture> m_modelTextures;
    String m_directory;
    SamplerState* m_textureSampler = nullptr;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    Vector<MeshTexture> loadMaterialTextures(aiMaterial* material,
                                             aiTextureType type);
  };
}