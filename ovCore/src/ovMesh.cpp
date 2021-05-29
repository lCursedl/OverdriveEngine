#include "ovMesh.h"
#include <ovGraphicsAPI.h>

namespace ovEngineSDK {
  Mesh::Mesh(Vector<MeshVertex>* vertex,
             Vector<uint32>* index,
             Vector<MeshTexture> texture) {
    m_vertices = vertex;
    m_indices = index;
    m_textures = texture;
    setUpMesh();
  }

  Mesh::~Mesh() {
    if (m_vertices) {
      if (m_vertices->size() > 0) {
        m_vertices->clear();
        delete m_vertices;
      }
    }
    if (m_indices) {
      if (m_indices->size() > 0) {
        m_indices->clear();
        delete m_indices;
      }
    }
    if (m_vertexBuffer) {
      delete m_vertexBuffer;
    }
    if (m_indexBuffer) {
      delete m_indexBuffer;
    }
    if (m_textures.size() > 0) {
      m_textures.clear();
    }
  }
  void Mesh::setUpMesh() {
    auto graphicAPI = &g_graphicsAPI();
    m_vertexBuffer = graphicAPI->createBuffer(m_vertices->data(),
                                              static_cast<int32>(sizeof(MeshVertex) * m_vertices->size()),
                                              BUFFER_TYPE::VERTEX_BUFFER);
    m_indexBuffer = graphicAPI->createBuffer(m_indices->data(),
                                             static_cast<int32>(sizeof(uint32) * m_indices->size()),
                                             BUFFER_TYPE::INDEX_BUFFER);
  }
  void Mesh::draw(SamplerState* sstate) {
    auto graphicAPI = &g_graphicsAPI();
    for (uint32 i = 0; i < m_textures.size(); i++) {
      graphicAPI->setSamplerState(0, m_textures[i].TextureMesh, sstate);
      graphicAPI->setTexture(i, m_textures[i].TextureMesh);
    }
    graphicAPI->setVertexBuffer(m_vertexBuffer, static_cast<int32>(sizeof(MeshVertex)), 0);
    graphicAPI->setIndexBuffer(m_indexBuffer);
    graphicAPI->drawIndexed(static_cast<int32>(m_indices->size()));
  }
}