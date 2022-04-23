#pragma once
#include <ovPrerequisitesCore.h>
#include <ovVolume.h>

namespace ovEngineSDK {
  class Parameters
  {
   public:
    Parameters() = default;
    ~Parameters() = default;

    double m_concavity = 0.001;
    double m_alpha = 0.05;
    double m_beta = 0.05;
    double m_minVolumePerCH = 0.0001;

    uint32 m_resolution = 100000;
    uint32 m_maxNumberVerticesPerCH = 64;
    uint32 m_planeDownSampling = 4;
    uint32 m_convexHullDownSampling = 4;
    uint32 m_pca = 0;
    uint32 m_mode = 0;
    uint32 m_convexHullApproximation = true;
    uint32 m_oclAcceleration = true;
    uint32 m_maxConvexHulls = 1024;
  };

  class VHACD
  {
   public:
   VHACD() = default;
   ~VHACD() = default;
   private:
    void
    Init();

    bool
    ComputeACD(const Vector<MeshVertex>& points,
                    const Vector<uint32>& indexes,
                    const Parameters& params);
   private:
    double m_overallProgress;
    double m_stageProgress;
    double m_operationProgress;
    Matrix4 m_rot;
    double m_volumeCH0;
    Vector3 m_baryCenter;
    SIZE_T m_dim;
    SPtr<Volume> m_volume;
    int32 m_ompNumProcessors;
  };
}