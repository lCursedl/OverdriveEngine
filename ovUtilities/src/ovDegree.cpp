#include "ovDegree.h"
#include "ovMath.h"

namespace ovEngineSDK {

  Degree::Degree(float d) {
    m_degree = d;
  }

  Degree::Degree(const Radian& r) {
    m_degree = r.toDegrees();
  }

  float Degree::toDegrees() const {
    return m_degree;
  }

  float Degree::toRadians() const
  {
      return m_degree * Math::DEG2RAD;
  }

  const Degree& Degree::operator+() const {
    return *this;
  }

  Degree Degree::operator+(const Degree& d) const {
    return Degree(m_degree + d.m_degree);
  }

  Degree& Degree::operator+=(const Degree& d) {
    m_degree += d.m_degree;
    return *this;
  }

  Degree Degree::operator-() const
  {
    return Degree(-m_degree);
  }

  Degree Degree::operator-(const Degree& d) const {
    return Degree(m_degree - d.m_degree);
  }

  bool Degree::operator<(const Degree& d) const {
    return m_degree < d.m_degree;
  }

  bool Degree::operator<=(const Degree& d) const {
    return m_degree <= d.m_degree;
  }

  bool Degree::operator==(const Degree& d) const {
    return m_degree == d.m_degree;
  }

  bool Degree::operator!=(const Degree& d) const {
    return m_degree != d.m_degree;
  }

  bool Degree::operator>=(const Degree& d) const {
    return m_degree >= d.m_degree;
  }

  bool Degree::operator>(const Degree& d) const {
    return m_degree > d.m_degree;
  }
}