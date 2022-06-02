#pragma once
#include <ovPrerequisitesCore.h>
#include <ovMesh.h>
#include <ovComponent.h>

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//#include <assimp/cimport.h>

struct aiAnimation;
struct aiScene;

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

    static SPtr<Model>
    load(String const& path, bool notexture = false);
    void
    transformBones(float delta, Vector<Matrix4>& Transforms);

    int32
    getMeshCount();

    void
    addMesh(const Vector<MeshVertex> vertices,
            const Vector<uint32> indices,
            const Vector<MeshTexture> textures);

    void
    addMesh(const SPtr<Mesh> mesh);

    static SPtr<Model>
    createBox();
    static SPtr<Model>
    createSphere(float radius = 1.f,
                 uint32 sectors = 36,
                 uint32 stacks = 18);
    static SPtr<Model>
    createCylinder(float bottomRadius = 1.f,
                   float topRadius = 1.f,
                   float height = 1.f,
                   uint32 sectors = 36,
                   uint32 stacks = 1);

    static SPtr<Model>
    loadOVFile(String const& path);

   public:
    Vector<SPtr<Mesh>> m_meshes;
    Vector<MeshTexture> m_modelTextures;
    String m_directory;
    SPtr<SamplerState> m_textureSampler;
    aiAnimation* m_modelAnimation = nullptr;
    aiScene* m_modelScene = nullptr;
    Matrix4 m_globalTransform;

    /*void processNode(aiNode* node, const aiScene* scene, bool texture);
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
    void calcInterpolatedScale(aiVector3D& Out, float animTime, const aiNodeAnim* nodeAnimation);*/

    friend class OmniverseOV;
  };
}