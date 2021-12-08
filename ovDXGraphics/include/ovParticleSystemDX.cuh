#pragma once
#include <ovPrerequisitesDX.h>
#include <ovParticleDX.h>

namespace ovEngineSDK {
  namespace EMISSION_SHAPE {
    enum E {
      
    };
  }
  class ParticleSystemDX
  {
   public:
    ParticleSystemDX() = default;
    ~ParticleSystemDX() = default;

    void
    init();

    void
    update(float delta);

    void
    destroy();

    Vector3 m_position;
    bool m_loop = false;
    Vector2 m_rangeLifeTime;
    float m_systemTime = 0.f;
    Vector2 m_particlesPerSec;
    int32 m_maxParticles;
    ParticleDX m_particles;
  };
}