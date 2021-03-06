#pragma once

#ifndef _INC_DEGREE_H_
# define _INC_DEGREE_H_
#endif // !_INC_DEGREE_H_

#include "ovPlatformDefines.h"

namespace ovEngineSDK {
  class Radian;

  class OV_UTILITY_EXPORT Degree
  {
    public:
    Degree() = default;

    Degree(float d);

    Degree&
    operator = (const Degree& d) = default;

    Degree(const Radian& r);

    float
    toDegrees() const;

    float
    toRadians() const;

    const Degree&
    operator+() const;

    Degree
    operator+(const Degree& d) const;

    Degree&
    operator+=(const Degree& d);

    Degree
    operator-() const;

    Degree
    operator-(const Degree& d) const;

    bool
    operator<(const Degree& d) const;

    bool
    operator<=(const Degree& d) const;

    bool
    operator==(const Degree& d) const;

    bool
    operator!=(const Degree& d) const;

    bool
    operator>=(const Degree& d) const;

    bool
    operator>(const Degree& d) const;

   private:
    float m_degree = 0.0f;
  };
}