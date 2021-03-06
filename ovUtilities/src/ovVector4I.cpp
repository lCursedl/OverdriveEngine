#include "ovVector4I.h"

namespace ovEngineSDK {
  const Vector4I Vector4I::ZERO = Vector4I(0, 0, 0, 0);

  Vector4I::Vector4I(int32 InX, int32 InY, int32 InZ, int32 InW) {
    x = InX;
    y = InY;
    z = InZ;
    w = InW;
  }

  Vector4I::Vector4I(Vector2I& Vec, int32 Z, int32 W) {
    x = Vec.x;
    y = Vec.y;
    z = Z;
    w = W;
  }

  Vector4I::Vector4I(Vector3I& Vec, int32 W) {
    x = Vec.x;
    y = Vec.y;
    z = Vec.z;
    w = W;
  }

  Vector4I Vector4I::operator+(const Vector4I& Other) const {
    return Vector4I(*this) += Other;
  }

  Vector4I Vector4I::operator-(const Vector4I& Other) const {
    return Vector4I(*this) -= Other;
  }

  Vector4I Vector4I::operator/(const Vector4I& Other) const {
    return Vector4I(*this) /= Other;
  }

  Vector4I& Vector4I::operator+=(const Vector4I& Other) {
    x += Other.x;
    y += Other.y;
    z += Other.z;
    w += Other.w;
    return *this;
  }

  Vector4I& Vector4I::operator-=(const Vector4I& Other) {
    x -= Other.x;
    y -= Other.y;
    z -= Other.z;
    w -= Other.w;
    return *this;
  }

  Vector4I& Vector4I::operator/=(const Vector4I& Other) {
    x /= Other.x;
    y /= Other.y;
    z /= Other.z;
    w /= Other.w;
    return *this;
  }

  bool Vector4I::operator==(const Vector4I& Other) const {
    return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
  }

  bool Vector4I::operator!=(const Vector4I& Other) const {
    return x != Other.x && y != Other.y && z != Other.z && w != Other.w;
  }

  Vector4I Vector4I::operator*(int32 Scale) const {
    return Vector4I(*this) *= Scale;
  }

  Vector4I Vector4I::operator/(int32 Divisor) const {
    return Vector4I(*this) /= Divisor;
  }

  Vector4I& Vector4I::operator*=(int32 Scale) {
    x *= Scale;
    y *= Scale;
    z *= Scale;
    w *= Scale;
    return *this;
  }

  Vector4I& Vector4I::operator/=(int32 Divisor) {
    x /= Divisor;
    y /= Divisor;
    z /= Divisor;
    w /= Divisor;
    return *this;
  }

  int32 Vector4I::dot(const Vector4I& vec) const {
    return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
  }

  Vector4I Vector4I::cross(const Vector4I& vec) const {
    return Vector4I(y * vec.z - z * vec.y,
                    z * vec.x - x * vec.z,
                    x * vec.y - y * vec.x,
                    0);
  }

  int32 Vector4I::magnitude() {
    return static_cast<int32>(Math::sqrt(static_cast<float>(x * x + y * y + z * z + w * w)));
  }
}