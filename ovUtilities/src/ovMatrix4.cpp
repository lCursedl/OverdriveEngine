#include "ovMatrix4.h"

namespace ovEngineSDK {
  
  const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 1.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 1.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 1.0f);

  Matrix4::Matrix4(float _xx, float _xy, float _xz, float _xw,
                   float _yx, float _yy, float _yz, float _yw,
                   float _zx, float _zy, float _zz, float _zw,
                   float _wx, float _wy, float _wz, float _ww) {
    xVector.x = _xx; xVector.y = _xy; xVector.z = _xz; xVector.w = _xw;
    yVector.x = _yx; yVector.y = _yy; yVector.z = _yz; yVector.w = _yw;
    zVector.x = _zx; zVector.y = _zy; zVector.z = _zz; zVector.w = _zw;
    wVector.x = _wx; wVector.y = _wy; wVector.z = _wz; wVector.w = _ww;
  }

  Matrix4::Matrix4(const Vector4& VecX,
                   const Vector4& VecY,
                   const Vector4& VecZ,
                   const Vector4& VecW) {
    xVector = VecX;
    yVector = VecY;
    zVector = VecZ;
    wVector = VecW;
  }

  Matrix4::Matrix4(const Vector3& VecX,
                   const Vector3& VecY,
                   const Vector3& VecZ,
                   const Vector3& VecW) {
    xVector.x = VecX.x; xVector.y = VecX.y; xVector.z = VecX.z; xVector.w = 0.0f;
    yVector.x = VecY.x; yVector.y = VecY.y; yVector.z = VecY.z; yVector.w = 0.0f;
    zVector.x = VecZ.x; zVector.y = VecZ.y; zVector.z = VecZ.z; zVector.w = 0.0f;
    wVector.x = VecW.x; wVector.y = VecW.y; wVector.z = VecW.z; wVector.w = 1.0f;
  }

  Matrix4 ovEngineSDK::Matrix4::operator+(const Matrix4& Mat) const {
    return Matrix4(*this) += Mat;
  }

  Matrix4 Matrix4::operator-(const Matrix4& Mat) const {
    return Matrix4(*this) -= Mat;
  }

  Matrix4 Matrix4::operator*(const Matrix4& Mat) const {
    Matrix4 T(Mat.transpose());
    return Matrix4(xVector.cross(T.xVector),
                   yVector.cross(T.yVector),
                   zVector.cross(T.zVector),
                   wVector.cross(T.wVector));
  }

  Matrix4 Matrix4::operator+=(const Matrix4& Mat) {
    xVector += Mat.xVector;
    yVector += Mat.yVector;
    zVector += Mat.zVector;
    wVector += Mat.zVector;
    return *this;
  }

  Matrix4 Matrix4::operator-=(const Matrix4& Mat) {
    xVector += Mat.xVector;
    yVector += Mat.yVector;
    zVector += Mat.zVector;
    wVector += Mat.zVector;
    return *this;
  }

  Matrix4 Matrix4::operator*=(const Matrix4& Mat) {
    *this = *this * Mat;
    return Matrix4();
  }

  bool Matrix4::operator==(const Matrix4& Mat) {
    return xVector == Mat.xVector &&
           yVector == Mat.yVector &&
           zVector == Mat.zVector &&
           wVector == Mat.wVector;
  }

  bool Matrix4::operator!=(const Matrix4& Mat) {
    return !(*this == Mat);
  }

  Matrix4 Matrix4::transpose() const {
    Matrix4 Result;
    Result.xVector.x = xVector.x;
    Result.xVector.y = yVector.x;
    Result.xVector.z = zVector.x;
    Result.xVector.w = wVector.x;

    Result.yVector.x = xVector.y;
    Result.yVector.y = yVector.y;
    Result.yVector.z = zVector.y;
    Result.yVector.w = wVector.y;

    Result.zVector.x = xVector.z;
    Result.zVector.y = yVector.z;
    Result.zVector.z = zVector.z;
    Result.zVector.w = wVector.z;

    Result.wVector.x = xVector.w;
    Result.wVector.y = yVector.w;
    Result.wVector.z = zVector.w;
    Result.wVector.w = wVector.w;
    return Result;
  }
}