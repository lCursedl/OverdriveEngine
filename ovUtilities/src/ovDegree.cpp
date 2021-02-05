#include "ovDegree.h"
#include "ovRadian.h"

namespace ovEngineSDK {

  Degree::Degree(const Radian& r)
  {
  }

  Degree& Degree::operator=(const float& f) {
    m_degree = f;
    return *this;
  }

  Degree& Degree::operator=(const Radian& r) {
    m_degree = r.toDegrees();
  }

  float Degree::toDegrees() const {
    return m_degree;
  }

  const Degree& Degree::operator+() const {
    return *this;
  }

  Degree Degree::operator+(const Degree& d) const {
    return Degree(m_degree + d.m_degree);
  }

  Degree Degree::operator+(const Radian& r) const {
    return Degree(m_degree + r.toDegrees());
  }

  Degree& Degree::operator+=(const Degree& d) {
    m_degree += d.m_degree;
    return *this;
  }

  Degree& Degree::operator+=(const Radian& r) {
    m_degree += r.toDegrees();
    return *this;
  }

  Degree Degree::operator-() const
  {
    return Degree(-m_degree);
  }

  Degree Degree::operator-(const Degree& d) const {
    return Degree(m_degree - d.m_degree);
  }

  Degree Degree::operator-(const Radian& r) const
  {
    return Degree(m_degree - r.toDegrees());
  }

  Degree& Degree::operator-=(const Radian& r) {
    m_degree -= r.toDegrees();
    return *this;
  }

  Degree Degree::operator*(float f) const {
    return Degree(m_degree * f);
  }

  Degree Degree::operator*(const Degree& f) const {
    return Degree(m_degree * f.m_degree);
  }

  Degree& Degree::operator*=(float f) {
    m_degree *= f;
    return *this;
  }

  Degree Degree::operator/(float f) const {
    return Degree(m_degree / f);
  }

  Degree& Degree::operator/=(float f) {
    m_degree /= f;
    return *this;
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

  bool Degree::operator<(const float& v) const {
    return m_degree < v;
  }

  bool Degree::operator<=(const float& v) const {
    return m_degree <= v;
  }

  bool Degree::operator==(const float& v) const {
    return m_degree == v;
  }

  bool Degree::operator!=(const float& v) const {
    return m_degree != v;
  }

  bool Degree::operator>=(const float& v) const {
    return m_degree >= v;
  }

  bool Degree::operator>(const float& v) const {
    return m_degree > v;
  }

}
