#include <ovVolume.h>

namespace ovEngineSDK {
  Volume::Volume() {
    m_dim[0] = m_dim[1] = m_dim[2] = 0;
    m_minBB = Vector3::ZERO;
    m_minBB = Vector3(1.f, 1.f, 1.f);
    m_numVoxelsOnSurface = 0;
    m_numVoxelsInsideSurface = 0;
    m_numVoxelsOutsideSurface = 0;
    m_scale = 1.f;
  }

  void
  Volume::Voxelize(const Vector<MeshVertex>& points,
                   const Vector<uint32>& triangles,
                   const uint32 dim,
                   const Vector3& barycenter,
                   const Matrix4& rot) {
    if (points.empty()) {
      return;
    }
    computeBB(points, barycenter, rot);

    Vector3 d(m_maxBB.x - m_minBB.x,
              m_maxBB.y - m_minBB.y,
              m_maxBB.z - m_minBB.z);
    float r;
    if (d.x >= d.y && d.x >= d.z) {
      r = d.x;
      m_dim[0] = dim;
      m_dim[1] = 2 + static_cast<SIZE_T>(dim * d.y / d.x);
      m_dim[2] = 2 + static_cast<SIZE_T>(dim * d.z / d.x);
    }
    else if (d.y >= d.x && d.y >= d.z) {
      r = d.y;
      m_dim[1] = dim;
      m_dim[0] = 2 + static_cast<SIZE_T>(dim * d.x / d.y);
      m_dim[2] = 2 + static_cast<SIZE_T>(dim * d.z / d.y);
    }
    else {
      r = d.z;
      m_dim[2] = dim;
      m_dim[0] = 2 + static_cast<SIZE_T>(dim * d.x / d.z);
      m_dim[1] = 2 + static_cast<SIZE_T>(dim * d.y / d.z);
    }
    m_scale = r / (dim - 1);
    float invScale = (dim - 1) / r;
  }
  void
  Volume::computeBB(const Vector<MeshVertex>& points,
                    const Vector3& barycenter,
                    const Matrix4& rot) {
    Vector3 pt = computeAlignedPoints(points[0], barycenter, rot);
    m_maxBB = pt;
    m_minBB = pt;
    for (auto& temp : points) {
      pt = computeAlignedPoints(temp, barycenter, rot);
      //Minimum
      m_minBB.x = pt.x < m_minBB.x ? pt.x : m_minBB.x;
      m_minBB.y = pt.y < m_minBB.y ? pt.y : m_minBB.y;
      m_minBB.z = pt.z < m_minBB.z ? pt.z : m_minBB.z;
      //Maximum
      m_maxBB.x = pt.x < m_maxBB.x ? pt.x : m_maxBB.x;
      m_maxBB.y = pt.y < m_maxBB.y ? pt.y : m_maxBB.y;
      m_maxBB.z = pt.z < m_maxBB.z ? pt.z : m_maxBB.z;
    }

  }
  Vector3
  Volume::computeAlignedPoints(const MeshVertex& points,
                               const Vector3& barycenter,
                               const Matrix4& rot) {
    float x = points.Position.x - barycenter.x;
    float y = points.Position.y - barycenter.y;
    float z = points.Position.z - barycenter.z;
    return Vector3(rot.xVector.x * x + rot.yVector.x * y + rot.zVector.x * z,
                   rot.xVector.y * x + rot.yVector.y * y + rot.zVector.y * z,
                   rot.xVector.z * x + rot.yVector.z * y + rot.zVector.z * z);
  }
}