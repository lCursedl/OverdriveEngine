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

    constexpr Degree(const Degree& d) = default;

    constexpr explicit Degree(float d) : m_degree(d) {}

    constexpr Degree&
    operator = (const Degree& d) = default;

    Degree(const Radian& r);

    Degree&
    operator = (const float& f);

    Degree&
    operator = (const Radian& r);

    float
    toDegrees() const;

    float
    toRadians() const;

    void
    unwindDegrees();

    const Degree&
    operator+() const;

    Degree
    operator+(const Degree& d) const;

    Degree
    operator+(const Radian& r) const;

    Degree&
    operator+=(const Degree& d);

    Degree&
    operator+=(const Radian& r);

    Degree
    operator-() const;

    Degree
    operator-(const Degree& d) const;

    Degree
    operator-(const Radian& r) const;

    Degree&
    operator-=(const Radian& r);

    Degree
    operator*(float f) const;

    Degree
    operator*(const Degree& f) const;

    Degree&
    operator*=(float f);

    Degree
    operator/(float f) const;

    Degree&
    operator/=(float f);

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

    bool
    operator<(const float& v) const;

    bool
    operator<=(const float& v) const;

    bool
    operator==(const float& v) const;

    bool
    operator!=(const float& v) const;

    bool
    operator>=(const float& v) const;

    bool
    operator>(const float& v) const;

    private:
    float m_degree = 0.0f;
  };
}