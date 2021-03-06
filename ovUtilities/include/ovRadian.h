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

    Radian(const Radian&) = default;

    Radian(float r);

    Radian(const Degree& d);

    Radian&
    operator = (const Radian&) = default;

    float
    toDegrees() const;

    float
    toRadians() const;

    const Radian&
    operator + () const;

    Radian
    operator + (const Radian& r) const;

    Radian&
    operator += (const Radian& r);

    Radian
    operator - () const;

    Radian
    operator - (const Radian& r) const;

    Radian&
    operator -= (const Radian& r);

    Radian
    operator * (const Radian& f) const;

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

   private:
    float m_radian = 0.0f;
  };
}