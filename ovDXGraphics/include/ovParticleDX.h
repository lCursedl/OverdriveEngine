#pragma once
#include <ovPrerequisitesDX.h>
#include <ovVector3.h>

namespace ovEngineSDK {
  struct ParticleDX {
    Vector<bool> m_alive;
    Vector<float> m_lifeTime;
    Vector<float> m_maxLifeTime;
    Vector<Vector3> m_position;
  };
}