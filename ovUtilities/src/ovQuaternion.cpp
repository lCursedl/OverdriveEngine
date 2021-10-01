#include "ovQuaternion.h"
#include "ovMatrix4.h"

namespace ovEngineSDK {
  Quaternion::Quaternion(float InX, float InY, float InZ, float InW) {
    x = InX;
    y = InY;
    z = InZ;
    w = InW;
  }
  Quaternion::Quaternion(Vector3& Vec, float AngleRad) {
    const float half_a = 0.5f * AngleRad;
    float s = Math::sin(half_a);
    float c = Math::cos(half_a);

    x = s * Vec.x;
    y = s * Vec.y;
    z = s * Vec.z;
    w = c;
  }

  Quaternion
  Quaternion::inverse() const {
      return Quaternion(-x, -y, -z, w);
  }

  float
  Quaternion::magnitude() const {
    return Math::sqrt(Math::pow(w, 2) +
                      Math::pow(x, 2) +
                      Math::pow(y, 2) +
                      Math::pow(z, 2));
  }

  bool
  Quaternion::normalize() {
    float m = Math::pow(w, 2) + Math::pow(x, 2) + Math::pow(y, 2) + Math::pow(z, 2);
    if (m > 1.0f) {
      m = magnitude();
      w /= m;
      x /= m;
      y /= m;
      z /= m;
      return true;
    }
    return false;
  }

  Quaternion
  Quaternion::fromMat(const Matrix4& Mat) {
    Quaternion MatQuat(0.f, 0.f, 0.f, 0.f);
    float s = 0.f;
    float trace = Mat.xVector.x + Mat.yVector.y + Mat.zVector.z;
    if (0 < trace) {
      s = Math::sqrt(trace + 1.f) * 2.f;
      MatQuat.w = 0.25f * s;
      MatQuat.x = (Mat.zVector.y - Mat.yVector.z) / s;
      MatQuat.y = (Mat.xVector.z - Mat.zVector.x) / s;
      MatQuat.z = (Mat.yVector.x - Mat.xVector.y) / s;
    }
    else if (Mat.xVector.x > Mat.yVector.y && Mat.xVector.x > Mat.zVector.z) {
      s = Math::sqrt(1.f + Mat.xVector.x - Mat.yVector.y - Mat.zVector.z) * 2.f;
      MatQuat.w = (Mat.zVector.y - Mat.yVector.z) / s;
      MatQuat.x = 0.25f * s;
      MatQuat.y = (Mat.xVector.y + Mat.yVector.x) / s;
      MatQuat.z = (Mat.xVector.z + Mat.zVector.x) / s;
    }
    else if (Mat.yVector.y > Mat.zVector.z) {
      s = Math::sqrt(1.f + Mat.yVector.y - Mat.xVector.x - Mat.zVector.z) * 2.f;
      MatQuat.w = (Mat.xVector.z - Mat.zVector.x) / s;
      MatQuat.x = (Mat.xVector.y + Mat.yVector.x) / s;
      MatQuat.y = 0.25f * s;
      MatQuat.z = (Mat.yVector.z + Mat.zVector.y) / s;
    }
    else {
      s = Math::sqrt(1.f + Mat.zVector.z - Mat.xVector.x - Mat.yVector.y) * 2;
      MatQuat.w = (Mat.yVector.x - Mat.xVector.y) / s;
      MatQuat.x = (Mat.xVector.z + Mat.zVector.x) / s;
      MatQuat.y = (Mat.yVector.z + Mat.zVector.y) / s;
      MatQuat.z = 0.25f * s;
    }
    return MatQuat;
  }

  Quaternion
  Quaternion::conjugate(const Quaternion& Quat) {
      return Quaternion(-Quat.x, -Quat.y, -Quat.z, Quat.w);
  }

  Quaternion
  Quaternion::operator*(const Quaternion& Q) const {
    return Quaternion((w * Q.x) + (x * Q.w) + (y * Q.z) - (z * Q.y),
                      (w * Q.y) - (x * Q.z) + (y * Q.w) + (z * Q.x),
                      (w * Q.z) + (x * Q.y) - (y * Q.x) + (z * Q.w),
                      (w * Q.w) - (x * Q.x) - (y * Q.y) - (z * Q.z));
  }
}
