#include "ovMesh.h"
#include <ovGraphicsAPI.h>

namespace ovEngineSDK {
  Mesh::Mesh(Vector<MeshVertex>* vertex,
             Vector<uint32>* index,
             Vector<MeshTexture> texture,
             aiMesh* mesh) {
    m_vertices = vertex;
    m_indices = index;
    m_textures = texture;
    m_numBones = 0;
    for (uint32 i = 0; i < mesh->mNumBones; i++) {
      uint32 boneIndex = 0;
      String boneName(mesh->mBones[i]->mName.data);
      if (m_boneMapping.find(boneName) == m_boneMapping.end()) {
        boneIndex = m_numBones;
        m_numBones++;
        BoneInfo bInfo;
        m_boneInfo.push_back(bInfo);
      }
      else {
        boneIndex = m_boneMapping[boneName];
      }
      m_boneMapping[boneName] = boneIndex;
      Matrix4 tempMatrix;
      tempMatrix.xVector.x = mesh->mBones[i]->mOffsetMatrix.a1;
      tempMatrix.xVector.y = mesh->mBones[i]->mOffsetMatrix.a2;
      tempMatrix.xVector.z = mesh->mBones[i]->mOffsetMatrix.a3;
      tempMatrix.xVector.w = mesh->mBones[i]->mOffsetMatrix.a4;

      tempMatrix.yVector.x = mesh->mBones[i]->mOffsetMatrix.b1;
      tempMatrix.yVector.y = mesh->mBones[i]->mOffsetMatrix.b2;
      tempMatrix.yVector.z = mesh->mBones[i]->mOffsetMatrix.b3;
      tempMatrix.yVector.w = mesh->mBones[i]->mOffsetMatrix.b4;

      tempMatrix.zVector.x = mesh->mBones[i]->mOffsetMatrix.c1;
      tempMatrix.zVector.y = mesh->mBones[i]->mOffsetMatrix.c2;
      tempMatrix.zVector.z = mesh->mBones[i]->mOffsetMatrix.c3;
      tempMatrix.zVector.w = mesh->mBones[i]->mOffsetMatrix.c4;

      tempMatrix.wVector.x = mesh->mBones[i]->mOffsetMatrix.d1;
      tempMatrix.wVector.y = mesh->mBones[i]->mOffsetMatrix.d2;
      tempMatrix.wVector.z = mesh->mBones[i]->mOffsetMatrix.d3;
      tempMatrix.wVector.w = mesh->mBones[i]->mOffsetMatrix.d4;

      m_boneInfo[boneIndex].BoneOffset = tempMatrix.transpose();

      for (int32 j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
        for (int32 k = 0; k < NUM_BONES_PERVERTEX; k++) {
          m_vertices->at(mesh->mBones[i]->mWeights[j].mVertexId).BoneID[k] = boneIndex;
          m_vertices->at(mesh->mBones[i]->mWeights[j].mVertexId).Weights[k] = 
                                                         mesh->mBones[i]->mWeights[j].mWeight;
        }
      }
    }
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

  void VertexBoneData::addBoneData(uint32 boneID, float weight) {
    for (uint32 i = 0; i < NUM_BONES_PERVERTEX; i++) {
      if (Weights[i] == 0.f) {
        BoneID[i] = boneID;
        Weights[i] = weight;
        return;
      }
    }
  }
}