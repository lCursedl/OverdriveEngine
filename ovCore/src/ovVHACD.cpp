#include "ovVHACD.h"

namespace ovEngineSDK {
  void
  VHACD::Init() {
    m_rot = Matrix4::IDENTITY;
    m_dim = 64;
    m_volume.reset();
    m_volumeCH0 = 0.0;
    m_overallProgress = 0.0;
    m_stageProgress = 0.0;
    m_baryCenter = Vector3::ZERO;
  }
  bool
  VHACD::ComputeACD(const Vector<MeshVertex>& points,
                    const Vector<uint32>& indexes,
                    const Parameters& params) {
    m_stageProgress = 0.0;
    std::ostringstream msg;
    
    return false;
  }
}