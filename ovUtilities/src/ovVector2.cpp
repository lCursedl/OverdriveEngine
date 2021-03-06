#include "ovVector2.h"
#include "ovVector3.h"

namespace ovEngineSDK {

  const Vector2 Vector2::ZERO = Vector2(0, 0);

  Vector2::Vector2(float InX, float InY) {
    x = InX;
    y = InY;
  }

  Vector2::Vector2(Vector2I In) {
    x = static_cast<float>(In.x);
    y = static_cast<float>(In.y);
  }

  Vector2
  Vector2::operator+(const Vector2& Other) const {
    return Vector2(*this) += Other;
  }

  Vector2
  Vector2::operator-(const Vector2& Other) const {
    return Vector2(*this) -= Other;
  }

  Vector2
  Vector2::operator/(const Vector2& Other) const {
    return Vector2(*this) /= Other;
  }

  Vector2&
  Vector2::operator+=(const Vector2& Other) {
    x += Other.x;
    y += Other.y;
    return *this;
  }

  Vector2&
  Vector2::operator-=(const Vector2& Other) {
    x -= Other.x;
    y -= Other.y;
    return *this;
  }

  Vector2&
  Vector2::operator/=(const Vector2& Other) {
    x /= Other.x;
    y /= Other.y;
    return *this;
  }

  bool
  Vector2::operator==(const Vector2& Other) const {
    return x == Other.x && y == Other.y;
  }

  bool
  Vector2::operator!=(const Vector2& Other) const {
    return x != Other.x && y != Other.y;
  }

  Vector2
  Vector2::operator*(float Scale) const {
    return Vector2(*this) *= Scale;
  }

  Vector2
  Vector2::operator/(float Divisor) const {
    return Vector2(*this) /= Divisor;
  }

  Vector2&
  Vector2::operator*=(float Scale) {
    x *= Scale;
    y *= Scale;
    return *this;
  }

  Vector2&
  Vector2::operator/=(float Divisor) {
    x /= Divisor;
    y /= Divisor;
    return *this;
  }

  float Vector2::dot(const Vector2& vec) const {
    return x * vec.x + y * vec.y;
  }

  float Vector2::magnitude() {
    return std::sqrtf(x * x + y * y);
  }
}