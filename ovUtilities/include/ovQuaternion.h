#pragma once

#include "ovPrerequisitesUtil.h"
#include "ovVector3.h"
#include "ovMath.h"

namespace ovEngineSDK {
  
  class Matrix4;

  class OV_UTILITY_EXPORT Quaternion
  {
   public:
    Quaternion() = default;
    Quaternion(float InX, float InY, float InZ, float InW);
    Quaternion(Vector3& Vec, float AngleRad);
    Quaternion
    inverse() const;

    float
    magnitude() const;

    bool
    normalize();

    static Quaternion
    fromMat(const Matrix4& Mat);

    static Quaternion
    conjugate(const Quaternion& Quat);

    static Quaternion
    fromEuler(const Vector3& Eul);

    Quaternion
    operator * (const Quaternion & Q) const;

   public:
    float x;
    float y;
    float z;
    float w;
  };
}