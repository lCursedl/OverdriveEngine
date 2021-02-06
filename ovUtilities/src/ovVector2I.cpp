#include "ovVector2I.h"

namespace ovEngineSDK {
  Vector2I::Vector2I(int32 InX, int32 InY) {
    x = InX;
    y = InY;
  }

  Vector2I
  Vector2I::operator+(const Vector2I& Other) const {
    return Vector2I(*this) += Other;
  }

  Vector2I
  Vector2I::operator-(const Vector2I& Other) const {
    return Vector2I(*this) -= Other;
  }

  Vector2I
  Vector2I::operator/(const Vector2I& Other) const {
    return Vector2I(*this) /= Other;
  }

  Vector2I&
  Vector2I::operator+=(const Vector2I& Other) {
    x += Other.x;
    y += Other.y;
    return *this;
  }

  Vector2I&
  Vector2I::operator-=(const Vector2I& Other) {
    x -= Other.x;
    y -= Other.y;
    return *this;
  }

  Vector2I&
  Vector2I::operator/=(const Vector2I& Other) {
    x /= Other.x;
    y /= Other.y;
    return *this;
  }

  bool
  Vector2I::operator==(const Vector2I& Other) const {
    return x == Other.x && y == Other.y;
  }

  bool
  Vector2I::operator!=(const Vector2I& Other) const {
    return x != Other.x && y != Other.y;
  }

  Vector2I
  Vector2I::operator*(int32 Scale) const {
    return Vector2I(*this) *= Scale;
  }

  Vector2I
  Vector2I::operator/(int32 Divisor) const {
    return Vector2I(*this) /= Divisor;
  }

  Vector2I&
  Vector2I::operator*=(int32 Scale) {
    x *= Scale;
    y *= Scale;
    return *this;
  }

  Vector2I&
  Vector2I::operator/=(int32 Divisor) {
    x /= Divisor;
    y /= Divisor;
    return *this;
  }

  int32 Vector2I::dot(const Vector2I& vec) const {
    return x * vec.x + y * vec.y;
  }

  int32 Vector2I::magnitude(){
      return static_cast<int32>(std::sqrt(x * x + y * y));
  }
}