#pragma once
#include <ovPrerequisitesCore.h>
#include <ovMesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>


namespace ovEngineSDK {
  class OV_CORE_EXPORT Model
  {
   public:
    Model() = default;
    ~Model();

    void draw();
    void load(String const& path);
    void transformBones(float delta, Vector<Matrix4>& Transforms);
   private:
    Vector<Mesh*> m_meshes;
    Vector<MeshTexture> m_modelTextures;
    String m_directory;
    SamplerState* m_textureSampler = nullptr;
    aiAnimation* m_modelAnimation = nullptr;
    aiScene* m_modelScene = nullptr;
    Matrix4 m_globalTransform;

    void processNode(aiNode* node, const aiScene* scene);
    Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
    Vector<MeshTexture> loadMaterialTextures(aiMaterial* material,
                                             aiTextureType type);
    const aiNodeAnim* findAnimationNode(const aiAnimation* anim, const String node);
    void readNodeHierarchy(float animTime,
                           const aiNode* node,
                           const Matrix4 tParent,
                           Mesh* modelMesh);
    uint32 findPosition(float animTime, const aiNodeAnim* nodeAnimation);
    uint32 findRotation(float animTime, const aiNodeAnim* nodeAnimation);
    uint32 findScaling(float animTime, const aiNodeAnim* nodeAnimation);
    void calcInterpolatedPos(aiVector3D& Out, float animTime, const aiNodeAnim* nodeAnimation);
    void calcInterpolatedRot(aiQuaternion& Out, float animTime, const aiNodeAnim* nodeAnimation);
    void calcInterpolatedScale(aiVector3D& Out, float animTime, const aiNodeAnim* nodeAnimation);
  };
}