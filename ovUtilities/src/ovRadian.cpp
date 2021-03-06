#include "ovRadian.h"
#include "ovMath.h"

namespace ovEngineSDK {

  Radian::Radian(float r) {
    m_radian = r;
  }

  Radian::Radian(const Degree& d) {
    m_radian = d.toRadians();
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

  Radian&
  Radian::operator+=(const Radian& r) {
    m_radian += r.m_radian;
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

  Radian& Radian::operator-=(const Radian& r) {
    m_radian -= r.m_radian;
    return *this;
  }

  Radian Radian::operator*(const Radian& f) const {
    return Radian(m_radian * f.m_radian);
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
}