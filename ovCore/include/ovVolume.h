#pragma once

#include <ovPrerequisitesCore.h>
#include <ovModel.h>
namespace ovEngineSDK {
  
  class OV_CORE_EXPORT PrimitiveSet
  {
   public:
    
   private:
    
  };

  class OV_CORE_EXPORT Volume
  {
   public:
    Volume();
    ~Volume() = default;

    
    void
    Voxelize(const Vector<MeshVertex>& points,
             const Vector<uint32>& triangles,
             const uint32 dim,
             const Vector3& barycenter,
             const Matrix4& rot);
    void
    computeBB(const Vector<MeshVertex>& points,
              const Vector3& barycenter,
              const Matrix4& rot);
    Vector3
    computeAlignedPoints(const MeshVertex& points,
                         const Vector3& barycenter,
                         const Matrix4& rot);
    
   private:
    Vector3 m_minBB;
    Vector3 m_maxBB;
    SIZE_T m_dim[3];
    float m_scale;
    SPtr<uint8> m_data;
    SIZE_T m_numVoxelsOnSurface;
    SIZE_T m_numVoxelsInsideSurface;
    SIZE_T m_numVoxelsOutsideSurface;
  };
}