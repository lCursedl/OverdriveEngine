#include "ovVector4.h"

namespace ovEngineSDK {
  const Vector4 Vector4::ZERO = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

  Vector4::Vector4(float InX, float InY, float InZ, float InW) {
    x = InX;
    y = InY;
    z = InZ;
    w = InW;
  }

  Vector4::Vector4(Vector2& Vec, float Z, float W) {
    x = Vec.x;
    y = Vec.y;
    z = Z;
    w = W;
  }

  Vector4::Vector4(Vector3& Vec, float W) {
    x = Vec.x;
    y = Vec.y;
    z = Vec.z;
    w = W;
  }

  Vector4::Vector4(Vector4I& Vec) {
    x = static_cast<float>(Vec.x);
    y = static_cast<float>(Vec.y);
    z = static_cast<float>(Vec.z);
    w = static_cast<float>(Vec.w);
  }

  Vector4 Vector4::operator+(const Vector4& Other) const {
    return Vector4(*this) += Other;
  }

  Vector4 Vector4::operator-(const Vector4& Other) const {
    return Vector4(*this) -= Other;
  }

  Vector4 Vector4::operator/(const Vector4& Other) const {
    return Vector4(*this) /= Other;
  }

  Vector4& Vector4::operator+=(const Vector4& Other) {
    x += Other.x;
    y += Other.y;
    z += Other.z;
    w += Other.w;
    return *this;
  }

  Vector4& Vector4::operator-=(const Vector4& Other) {
    x -= Other.x;
    y -= Other.y;
    z -= Other.z;
    w -= Other.w;
    return *this;
  }

  Vector4& Vector4::operator/=(const Vector4& Other) {
    x /= Other.x;
    y /= Other.y;
    z /= Other.z;
    w /= Other.w;
    return *this;
  }

  bool Vector4::operator==(const Vector4& Other) const {
    return x == Other.x && y == Other.y && z == Other.z && w == Other.w;
  }

  bool Vector4::operator!=(const Vector4& Other) const {
    return x != Other.x && y != Other.y && z != Other.z && w != Other.w;
  }

  Vector4 Vector4::operator*(float Scale) const {
    return Vector4(*this) *= Scale;
  }

  Vector4 Vector4::operator/(float Divisor) const {
    return Vector4(*this) /= Divisor;
  }

  Vector4& Vector4::operator*=(float Scale) {
    x *= Scale;
    y *= Scale;
    z *= Scale;
    w *= Scale;
    return *this;
  }

  Vector4& Vector4::operator/=(float Divisor) {
    x /= Divisor;
    y /= Divisor;
    z /= Divisor;
    w /= Divisor;
    return *this;
  }

  float Vector4::dot(const Vector4& vec) const {
    return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
  }

  Vector4 Vector4::cross(const Vector4& vec) const {
    return Vector4(y * vec.z - z * vec.y,
                   z * vec.x - x * vec.z,
                   x * vec.y - y * vec.x,
                   0.0f);
  }

  float Vector4::magnitude() {
    return Math::sqrt(x * x + y * y + z * z + w * w);
  }
}