#pragma once

#include "ovPrerequisitesUtil.h"
#include "ovVector3.h"
#include "ovRadian.h"

namespace ovEngineSDK {
  class OV_UTILITY_EXPORT Quaternion
  {
   public:
    Quaternion() = default;
    Quaternion(float InX, float InY, float InZ, float InW);
    Quaternion(Vector3& Vec, float AngleRad);
    Quaternion
    inverse() const;
   public:
    float x;
    float y;
    float z;
    float w;
  };
}