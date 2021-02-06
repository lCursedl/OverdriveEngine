#include "ovRadian.h"
#include "ovMath.h"

namespace ovEngineSDK {
  Radian::Radian(const Degree& d) : m_radian(d.toRadians()) {}

  Radian&
  Radian::operator=(const Degree& d) {
    m_radian = d.toRadians();
    return *this;
  }

  float
  Radian::toDegrees() const {
    return m_radian * Math::RAD2DEG;
  }

  float
  Radian::toRadians() const {
    return m_radian;
  }

  const Radian&
  Radian::operator+() const {
    return *this;
  }

  Radian
  Radian::operator+(const Radian& r) const {
    return Radian(m_radian + r.m_radian);
  }

  Radian
  Radian::operator+(const Degree& d) const {
    return Radian(m_radian + d.toRadians());
  }

  Radian&
  Radian::operator+=(const Radian& r) {
    m_radian += r.m_radian;
    return *this;
  }

  Radian&
  Radian::operator+=(const Degree& d) {
    m_radian += d.toRadians();
    return *this;
  }

  Radian
  Radian::operator-() const {
    return Radian(-m_radian);
  }

  Radian
  Radian::operator-(const Radian& r) const {
    return Radian(m_radian - r.m_radian);
  }

  Radian
  Radian::operator-(const Degree& d) const {
    return Radian(m_radian - d.toRadians());
  }

  Radian& Radian::operator-=(const Radian& r) {
    m_radian -= r.m_radian;
    return *this;
  }

  Radian& Radian::operator-=(const Degree& d) {
    m_radian -= d.toRadians();
    return *this;
  }

  Radian Radian::operator*(float f) const {
    return Radian(m_radian * f);
  }

  Radian Radian::operator*(const Radian& f) const {
    return Radian(m_radian * f.m_radian);
  }

  Radian& Radian::operator*=(float f) {
    m_radian *= f;
    return *this;
  }

  Radian Radian::operator/(float f) const {
    return Radian(m_radian / f);
  }

  Radian& Radian::operator/=(float f) {
    m_radian /= f;
    return *this;
  }

  bool Radian::operator<(const Radian& r) {
    return m_radian < r.m_radian;
  }

  bool Radian::operator<=(const Radian& r) const {
    return m_radian <= r.m_radian;
  }

  bool Radian::operator==(const Radian& r) const {
    return m_radian == r.m_radian;
  }

  bool Radian::operator!=(const Radian& r) const {
    return m_radian != r.m_radian;
  }

  bool Radian::operator>=(const Radian& r) const {
    return m_radian >= r.m_radian;
  }

  bool Radian::operator>(const Radian& r) const {
    return m_radian > r.m_radian;
  }

  bool Radian::operator<(const float& v) const {
    return m_radian < v;
  }

  bool Radian::operator<=(const float& v) const {
    return m_radian <= v;
  }

  bool Radian::operator==(const float& v) const {
    return m_radian == v;
  }

  bool Radian::operator!=(const float& v) const {
    return m_radian != v;
  }

  bool Radian::operator>=(const float& v) const {
    return m_radian >= v;
  }

  bool Radian::operator>(const float& v) const {
    return m_radian > v;
  }
}