#pragma once
#include <ovPrerequisitesCore.h>
#include <ovMesh.h>
#include <ovComponent.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

//class aiAnimation;
//class aiScene;
//class aiNodeAnim;
//class aiTextureType;
//class aiMaterial;
//class aiNode;
//class aiMesh;
//class aiVector3D;
//class aiQuaternion;

namespace ovEngineSDK {
  

  class OV_CORE_EXPORT Model : public Component
  {
   public:
    Model() = default;
    ~Model();

    void
    update(float delta)                                               override;

    void
    render()                                                          override;

    void
    load(String const& path, bool notexture = false);
    void
    transformBones(float delta, Vector<Matrix4>& Transforms);

    void
    getModelInfo(Vector<Vector3>& vertices,
                 Vector<Vector2I>& indices,
                 Vector<Vector3>& normals,
                 Vector<Vector2>& uvs);

    int32
    getMeshCount();

    void
    addMesh(const Vector<MeshVertex> vertices,
            const Vector<uint32> indices,
            const Vector<MeshTexture> textures);

    static SPtr<Model> createBox();
    static SPtr<Model> createSphere();
    static SPtr<Model> createCylinder();

   private:
    Vector<SPtr<Mesh>> m_meshes;
    Vector<MeshTexture> m_modelTextures;
    String m_directory;
    SPtr<SamplerState> m_textureSampler;
    aiAnimation* m_modelAnimation = nullptr;
    aiScene* m_modelScene = nullptr;
    Matrix4 m_globalTransform;

    void processNode(aiNode* node, const aiScene* scene, bool texture);
    SPtr<Mesh> processMesh(aiMesh* mesh, const aiScene* scene, bool texture);
    Vector<MeshTexture> loadMaterialTextures(aiMaterial* material,
                                             aiTextureType type,
                                             bool texture);
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

    friend class OmniverseOV;
  };
}