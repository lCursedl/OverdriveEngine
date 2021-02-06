#include "ovVector3I.h"

namespace ovEngineSDK {
  const Vector3I Vector3I::ZERO = Vector3I(0, 0, 0);

  Vector3I::Vector3I(int32 InX, int32 InY, int32 InZ) {
    x = InX;
    y = InY;
    z = InZ;
  }

  Vector3I::Vector3I(Vector2I& Vec, int32 Value) {
    x = Vec.x;
    y = Vec.y;
    z = Value;
  }

  Vector3I
  Vector3I::operator+(const Vector3I& Other) const {
    return Vector3I(*this) += Other;
  }

  Vector3I
  Vector3I::operator-(const Vector3I& Other) const {
    return Vector3I(*this) -= Other;
  }

  Vector3I
  Vector3I::operator/(const Vector3I& Other) const {
    return Vector3I(*this) /= Other;
  }

  Vector3I&
  Vector3I::operator+=(const Vector3I& Other) {
    x += Other.x;
    y += Other.y;
    z += Other.z;
    return *this;
  }

  Vector3I&
  Vector3I::operator-=(const Vector3I& Other) {
    x -= Other.x;
    y -= Other.y;
    z -= Other.z;
    return *this;
  }

  Vector3I&
  Vector3I::operator/=(const Vector3I& Other) {
    x /= Other.x;
    y /= Other.y;
    z /= Other.z;
    return *this;
  }

  bool
  Vector3I::operator==(const Vector3I& Other) const {
    return x == Other.x && y == Other.y && z == Other.z;
  }

  bool
  Vector3I::operator!=(const Vector3I& Other) const {
    return x != Other.x && y != Other.y && z != Other.z;
  }

  Vector3I
  Vector3I::operator*(int32 Scale) const {
    return Vector3I(*this) *= Scale;
  }

  Vector3I
  Vector3I::operator/(int32 Divisor) const {
    return Vector3I(*this) /= Divisor;
  }

  Vector3I&
  Vector3I::operator*=(int32 Scale) {
    x *= Scale;
    y *= Scale;
    z *= Scale;
    return *this;
  }

  Vector3I&
  Vector3I::operator/=(int32 Divisor) {
    x /= Divisor;
    y /= Divisor;
    z /= Divisor;
    return *this;
  }

  int32 Vector3I::dot(const Vector3I& vec) const {
    return x * vec.x + y * vec.y + z * vec.z;
  }

  int32 Vector3I::magnitude() {
    return static_cast<int32>(std::sqrt(x * x + y * y + z * z));
  }
}