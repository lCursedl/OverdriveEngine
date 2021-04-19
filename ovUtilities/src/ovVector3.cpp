#include "ovVector3.h"

namespace ovEngineSDK {
  const Vector3 Vector3::ZERO = Vector3(0.0f, 0.0f, 0.0f);

  Vector3::Vector3(float InX, float InY, float InZ) {
    x = InX;
    y = InY;
    z = InZ;
  }

  Vector3::Vector3(Vector2& Vec, float Value) {
    x = Vec.x;
    y = Vec.y;
    z = Value;
  }

  Vector3::Vector3(Vector3I In) {
    x = static_cast<float>(In.x);
    y = static_cast<float>(In.y);
    z = static_cast<float>(In.z);
  }

  Vector3
  Vector3::operator+(const Vector3& Other) const {
    return Vector3(*this) += Other;
  }

  Vector3
  Vector3::operator-(const Vector3& Other) const {
    return Vector3(*this) -= Other;
  }

  Vector3
  Vector3::operator/(const Vector3& Other) const {
    return Vector3(*this) /= Other;
  }

  Vector3&
  Vector3::operator+=(const Vector3& Other) {
    x += Other.x;
    y += Other.y;
    z += Other.z;
    return *this;
  }

  Vector3&
  Vector3::operator-=(const Vector3& Other) {
    x -= Other.x;
    y -= Other.y;
    z -= Other.z;
    return *this;
  }

  Vector3&
  Vector3::operator/=(const Vector3& Other) {
    x /= Other.x;
    y /= Other.y;
    z /= Other.z;
    return *this;
  }

  bool
  Vector3::operator==(const Vector3& Other) const {
    return x == Other.x && y == Other.y && z == Other.z;
  }

  bool
  Vector3::operator!=(const Vector3& Other) const {
    return x != Other.x && y != Other.y && z != Other.z;
  }

  Vector3
  Vector3::operator*(float Scale) const {
    return Vector3(*this) *= Scale;
  }

  Vector3
  Vector3::operator/(float Divisor) const {
    return Vector3(*this) /= Divisor;
  }

  Vector3&
  Vector3::operator*=(float Scale) {
    x *= Scale;
    y *= Scale;
    z *= Scale;
    return *this;
  }

  Vector3&
  Vector3::operator/=(float Divisor) {
    x /= Divisor;
    y /= Divisor;
    z /= Divisor;
    return *this;
  }

  Vector3
  Vector3::operator^(const Vector3& v) const {
      return Vector3(y * v.z - z * v.y,
                     z * v.x - x * v.z,
                     x * v.y - y * v.x);
  }

  Vector3 Vector3::operator-() const {
      return Vector3(-x, -y, -z);
  }

  float Vector3::dot(const Vector3& vec) const {
    return x * vec.x + y * vec.y + z * vec.z;
  }

  Vector3
  Vector3::cross(const Vector3& a, const Vector3& b) {
    return a ^ b;
  }

  float Vector3::magnitude() {
    return static_cast<float>(std::sqrtf(x * x + y * y + z * z));
  }

  bool Vector3::normalize(float tolerance) {
    const float SquareSum = (x * x) + (y * y) + (z * z);
    if (SquareSum > tolerance) {
      const float Scale = Math::invSqrt(SquareSum);
      x *= Scale;
      y *= Scale;
      z *= Scale;
      return true;
    }
    return false;
  }
}