#include "ovQuaternion.h"

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
}