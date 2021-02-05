#pragma once

#include "ovPlatformDefines.h"

#ifndef _INC_RADIAH_H_
# define _INC_RADIAH_H_
#endif // !_INC_RADIAH_H_

namespace ovEngineSDK {
  class Degree;

  class OV_UTILITY_EXPORT Radian
  {
   public:
    Radian() = default;

    constexpr Radian(const Radian&) = default;

    constexpr explicit Radian(float r) : m_radian(r) {}

    Radian(const Degree& d);

    constexpr Radian&
    operator=(const float& f) {
      m_radian = f;
      return *this;
    }

    constexpr Radian&
    operator = (const Radian&) = default;

    Radian&
    operator = (const Degree& d);

    float
    toDegrees() const;

    float
    toRadians() const;

    const Radian&
    operator + () const;

    Radian
    operator + (const Radian& r) const;

    Radian&
    operator + (const Degree& d) const;

    Radian&
    operator += (const Radian& r);

    Radian&
    operator += (const Degree &d);

    Radian
    operator - () const;

    Radian
    operator - (const Radian& r) const;

    Radian
    operator - (const Degree& d) const;

    Radian&
    operator -= (const Radian& r);

    Radian&
    operator -= (const Degree& d);

    Radian
    operator * (float f) const;

    Radian
    operator * (const Radian& f) const;

    Radian&
    operator *= (float f);

    Radian
    operator / (float f) const;

    Radian&
    operator /= (float f);

    bool
    operator < (const Radian& r);

    bool
    operator <= (const Radian& r) const;

    bool
    operator == (const Radian& r) const;

    bool
    operator != (const Radian& r) const;

    bool
    operator >= (const Radian& r) const;

    bool
    operator > (const Radian& r) const;

    bool
    operator < (const float& v) const;

    bool
    operator <= (const float& v) const;

    bool
    operator == (const float& v) const;

    bool
    operator != (const float& v) const;

    bool
    operator >= (const float& v) const;

    bool
    operator > (const float& v) const;

    private:
     float m_radian = 0.0f;
  };
}