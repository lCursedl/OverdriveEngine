#include "ovMath.h"

namespace ovEngineSDK {

  const float Math::PI = static_cast<float>(4.0 * std::atan(1.0f));
  const float Math::INV_PI = static_cast<float>(1.0f / PI);
  const float Math::HALF_PI = static_cast<float>(0.5f * PI);
  const float Math::DEG2RAD = static_cast<float>(PI / 180.0f);
  const float Math::RAD2DEG = static_cast<float>(180.0f / PI);
  const float Math::LOG2 = static_cast<float>(std::log(2.0f));

  const float Math::SMALL_NUMBER = (1.e-6f);
  const float Math::BIG_NUMBER = (3.4e+38f);

  int32
  Math::trunc(float F) {
    return static_cast<int32>(F);
  }

  float
  Math::truncFloat(float F) {
    return static_cast<float>(trunc(F));
  }

  float
  Math::floorFloat(float F) {
    return std::floorf(F);
  }

  int32
  Math::round(float F) {
    return static_cast<int32>(floor(F + 0.5f));
  }

  float
  Math::roundFloat(float F) {
    return floorFloat(F +0.5f);
  }

  int32
  Math::ceil(float F) {
    return trunc(std::ceilf(F));
  }

  float
  Math::ceilFloat(float F) {
    return std::ceilf(F);
  }

  float
  Math::fractional(float Value) {
    return Value - truncFloat(Value);
  }

  float
  Math::fmod(float X, float Y) {
    return std::fmodf(X, Y);
  }

  float
  Math::pow(float Base, float Exponent) {
    return std::powf(Base, Exponent);
  }

  float
  Math::exp(float Value) {
    return std::expf(Value);
  }

  float
  Math::logE(float Value) {
    return std::logf(Value);
  }

  float
  Math::log2(float Value) {
    return std::logf(Value) / LOG2;
  }

  float
  Math::logX(float Base, float Value) {
    return logE(Value) / logE(Base);
  }

  float
  Math::sqrt(float Value)
  {
    return std::sqrtf(Value);
  }

  Radian
  Math::sqrt(const Radian& Value) {
    return Radian(sqrt(Value.toRadians()));
  }

  Degree
  Math::sqrt(const Degree& Value) {
    return Degree(sqrt(Value.toDegrees()));
  }

  float
  Math::invSqrt(float F) {
    return 1.0f / std::sqrtf(F);
  }

  float
  Math::cos(float Value) {
    return std::cosf(Value);
  }

  float
  Math::sin(float Value) {
    return std::sinf(Value);
  }

  float
  Math::tan(float Value) {
    return std::tanf(Value);
  }

  float
  Math::cos(const Radian& Value) {
    return std::cosf(Value.toRadians());
  }

  float
  Math::sin(const Radian& Value) {
    return std::sinf(Value.toRadians());
  }

  float
  Math::tan(const Radian& Value) {
    return std::tanf(Value.toRadians());
  }

  float
  Math::cosh(float Value) {
    return std::coshf(Value);
  }

  float
  Math::sinh(float Value) {
    return std::sinh(Value);
  }

  float
  Math::tanh(float Value) {
    return std::tanh(Value);
  }

  Radian
  Math::acos(float Value) {
    if (-1.0f < Value) {
      if (1.0f > Value) {
        return Radian(std::acos(Value));
      }
      else {
        return Radian(0.0f);
      }
    }
    return Radian(PI);
  }

  Radian
  Math::asin(float Value) {
    if (-1.0f < Value) {
      if (1.0f > Value) {
        return Radian(std::asin(Value));
      }
      else {
        return Radian(HALF_PI);
      }
    }
    return Radian(-HALF_PI);
  }

  Radian
  Math::atan(float Value) {
    return Radian(std::atanf(Value));
  }

  Radian
  Math::atan2(float Y, float X) {
    return Radian(std::atan2f(Y, X));
  }

  float Math::cot(float Value) {
      return cos(Value) / sin(Value);
  }
}