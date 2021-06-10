#include "ovMatrix4.h"

namespace ovEngineSDK {
  
  const Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
                                            0.0f, 1.0f, 0.0f, 0.0f,
                                            0.0f, 0.0f, 1.0f, 0.0f,
                                            0.0f, 0.0f, 0.0f, 1.0f);

  Matrix4::Matrix4(float value) {
    xVector.x = value;
    xVector.y = value;
    xVector.z = value;
    xVector.w = value;

    yVector.x = value;
    yVector.y = value;
    yVector.z = value;
    yVector.w = value;

    zVector.x = value;
    zVector.y = value;
    zVector.z = value;
    zVector.w = value;
  }

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

  void Matrix4::operator=(const Matrix4& Mat) {
    xVector = Mat.xVector;
    yVector = Mat.yVector;
    zVector = Mat.zVector;
    wVector = Mat.wVector;
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

  LookAtMatrix::LookAtMatrix(const Vector3& EyePosition,
                             const Vector3& LookAtPosition,
                             const Vector3& UpVector) {
    Vector3 ZAxis = LookAtPosition - EyePosition;
    ZAxis.normalize();
    Vector3 XAxis = UpVector ^ ZAxis;
    XAxis.normalize();
    Vector3 YAxis = ZAxis ^ XAxis;

    xVector.x = (XAxis.x);
    xVector.y = (YAxis.x);
    xVector.z = (ZAxis.x);
    xVector.w = 0.f;

    yVector.x = (XAxis.y);
    yVector.y = (YAxis.y);
    yVector.z = (ZAxis.y);
    yVector.w = 0.f;

    zVector.x = (XAxis.z);
    zVector.y = (YAxis.z);
    zVector.z = (ZAxis.z);
    zVector.w = 0.f;

    Vector3 negEyePos = -EyePosition;
    wVector.x = negEyePos.dot(XAxis);
    wVector.y = negEyePos.dot(YAxis);
    wVector.z = negEyePos.dot(ZAxis);
    wVector.w = 1.f;
  }

  PerspectiveMatrix::PerspectiveMatrix(float FOV,
                                       float Width,
                                       float Height,
                                       float MinZ,
                                       float MaxZ) {
    float yScale = Math::cot(FOV / 2);
    float xScale = yScale / (Width / Height);

    xVector = Vector4(xScale, 0.f, 0.f, 0.f);
    yVector = Vector4(0.f, yScale, 0.f, 0.f);
    zVector = Vector4(0.f, 0.f, MaxZ / (MaxZ - MinZ), 1.f);
    wVector = Vector4(0.f, 0.f, -MinZ * MaxZ / (MaxZ - MinZ), 0.f);
  }
}