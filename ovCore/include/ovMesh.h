#pragma once

#include <ovPrerequisitesCore.h>
#include <ovVector2.h>
#include <ovVector3.h>
#include <ovVector4.h>

namespace ovEngineSDK {
  class Buffer;
  class Texture;
  class SamplerState;

  struct MeshVertex
  {
   public:
    Vector3 Position;
    Vector2 TexCoords;
    Vector3 Normal;
    Vector3 Tangent;
    Vector3 Bitangent;
  };

  struct MeshTexture
  {
   public:
    String Type;
    String Path;
    Texture* TextureMesh = nullptr;
  };

  class OV_CORE_EXPORT Mesh
  {
   public:
    Mesh(Vector<MeshVertex>* vertex,
         Vector<uint32>* index,
         Vector<MeshTexture> texture);
    ~Mesh();
    void setUpMesh();
    void draw(SamplerState* sstate);
   private:
    Vector<MeshVertex>*  m_vertices;
    Vector<uint32>*      m_indices;
    Vector<MeshTexture> m_textures;

    Buffer* m_vertexBuffer;
    Buffer* m_indexBuffer;
  };
}