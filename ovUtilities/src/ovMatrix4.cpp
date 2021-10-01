#include "ovMatrix4.h"
#include "ovQuaternion.h"

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

    wVector.x = value;
    wVector.y = value;
    wVector.z = value;
    wVector.w = value;
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
    Matrix4 T;
    T.xVector.x = xVector.x * Mat.xVector.x + xVector.y * Mat.yVector.x + xVector.z * Mat.zVector.x + xVector.w * Mat.wVector.x;
    T.xVector.y = xVector.x * Mat.xVector.y + xVector.y * Mat.yVector.y + xVector.z * Mat.zVector.y + xVector.w * Mat.wVector.y;
    T.xVector.z = xVector.x * Mat.xVector.z + xVector.y * Mat.yVector.z + xVector.z * Mat.zVector.z + xVector.w * Mat.wVector.z;
    T.xVector.w = xVector.x * Mat.xVector.w + xVector.y * Mat.yVector.w + xVector.z * Mat.zVector.w + xVector.w * Mat.wVector.w;

    T.yVector.x = yVector.x * Mat.xVector.x + yVector.y * Mat.yVector.x + yVector.z * Mat.zVector.x + yVector.w * Mat.wVector.x;
    T.yVector.y = yVector.x * Mat.xVector.y + yVector.y * Mat.yVector.y + yVector.z * Mat.zVector.y + yVector.w * Mat.wVector.y;
    T.yVector.z = yVector.x * Mat.xVector.z + yVector.y * Mat.yVector.z + yVector.z * Mat.zVector.z + yVector.w * Mat.wVector.z;
    T.yVector.w = yVector.x * Mat.xVector.w + yVector.y * Mat.yVector.w + yVector.z * Mat.zVector.w + yVector.w * Mat.wVector.w;

    T.zVector.x = zVector.x * Mat.xVector.x + zVector.y * Mat.yVector.x + zVector.z * Mat.zVector.x + zVector.w * Mat.wVector.x;
    T.zVector.y = zVector.x * Mat.xVector.y + zVector.y * Mat.yVector.y + zVector.z * Mat.zVector.y + zVector.w * Mat.wVector.y;
    T.zVector.z = zVector.x * Mat.xVector.z + zVector.y * Mat.yVector.z + zVector.z * Mat.zVector.z + zVector.w * Mat.wVector.z;
    T.zVector.w = zVector.x * Mat.xVector.w + zVector.y * Mat.yVector.w + zVector.z * Mat.zVector.w + zVector.w * Mat.wVector.w;

    T.wVector.x = wVector.x * Mat.xVector.x + wVector.y * Mat.yVector.x + wVector.z * Mat.zVector.x + wVector.w * Mat.wVector.x;
    T.wVector.y = wVector.x * Mat.xVector.y + wVector.y * Mat.yVector.y + wVector.z * Mat.zVector.y + wVector.w * Mat.wVector.y;
    T.wVector.z = wVector.x * Mat.xVector.z + wVector.y * Mat.yVector.z + wVector.z * Mat.zVector.z + wVector.w * Mat.wVector.z;
    T.wVector.w = wVector.x * Mat.xVector.w + wVector.y * Mat.yVector.w + wVector.z * Mat.zVector.w + wVector.w * Mat.wVector.w;

    return T;
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

  Matrix4
  Matrix4::scale(const Matrix4& Mat, const Vector3& Vec) {
    Matrix4 Result(0.f);
    Result.xVector.x = Mat.xVector.x * Vec.x;
    Result.yVector.y = Mat.yVector.y * Vec.y;
    Result.zVector.z = Mat.zVector.z * Vec.z;
    Result.wVector.w = 1.f;
    return Result;
  }

  Matrix4
  Matrix4::fromQuat(const Quaternion& Quat) {
    Matrix4 QuatMatrix(0.f);
    QuatMatrix.xVector.x = 1 - 2 * (Math::pow(Quat.y, 2) + Math::pow(Quat.z, 2));
    QuatMatrix.xVector.y = 2 * (Quat.x * Quat.y - Quat.w * Quat.z);
    QuatMatrix.xVector.z = 2 * (Quat.x * Quat.z + Quat.w * Quat.y);

    QuatMatrix.yVector.x = 2 * (Quat.x * Quat.y + Quat.w * Quat.z);
    QuatMatrix.yVector.y = 1 - 2 * (Math::pow(Quat.x, 2) + Math::pow(Quat.z, 2));
    QuatMatrix.yVector.z = 2 * (Quat.y * Quat.z - Quat.w * Quat.x);

    QuatMatrix.zVector.x = 2 * (Quat.x * Quat.z - Quat.w * Quat.y);
    QuatMatrix.zVector.y = 2 * (Quat.y * Quat.z + Quat.w * Quat.x);
    QuatMatrix.zVector.z = 1 - 2 * (Math::pow(Quat.x, 2) + Math::pow(Quat.y, 2));

    QuatMatrix.wVector.w = 1.f;
    return QuatMatrix;
  }

  Matrix4
  Matrix4::inverse() const {
    Matrix4 Result, Temp;

    Temp.xVector.x = zVector.z * wVector.w - zVector.w * wVector.z;
    Temp.xVector.y = yVector.z * wVector.w - yVector.w * wVector.z;
    Temp.xVector.z = yVector.z * zVector.w - yVector.w * zVector.z;

    Temp.yVector.x = zVector.z * wVector.w - zVector.w * wVector.z;
    Temp.yVector.y = xVector.z * wVector.w - xVector.w * wVector.z;
    Temp.yVector.z = xVector.z * zVector.w - xVector.w * zVector.z;

    Temp.zVector.x = yVector.z * wVector.w - yVector.w * wVector.z;
    Temp.zVector.y = xVector.z * wVector.w - xVector.w * wVector.z;
    Temp.zVector.z = xVector.z * yVector.w - xVector.w * yVector.z;

    Temp.wVector.x = yVector.z * zVector.w - yVector.w * zVector.z;
    Temp.wVector.y = xVector.z * zVector.w - xVector.w * zVector.z;
    Temp.wVector.z = xVector.x * yVector.w - xVector.w * yVector.z;

    float d0 = yVector.y * Temp.xVector.x - zVector.y * Temp.xVector.y + wVector.y * Temp.xVector.z;
    float d1 = xVector.y * Temp.yVector.x - zVector.y * Temp.yVector.y + wVector.y * Temp.yVector.z;
    float d2 = xVector.y * Temp.zVector.x - yVector.y * Temp.zVector.y + wVector.y * Temp.zVector.z;
    float d3 = xVector.y * Temp.wVector.x - yVector.y * Temp.wVector.y + zVector.y * Temp.wVector.z;

    float determinant = xVector.x * d0 - yVector.x * d1 + zVector.x * d2 - wVector.x * d3;
    const float rDet = 1.f / determinant;

    Result.xVector.x = rDet * d0;
    Result.xVector.y = -rDet * d1;
    Result.xVector.z = rDet * d2;
    Result.xVector.w = -rDet * d3;

    Result.yVector.x = -rDet * (yVector.x * Temp.xVector.x -
                                zVector.x * Temp.xVector.y +
                                wVector.x * Temp.xVector.z);
    Result.yVector.y = rDet * (xVector.x * Temp.yVector.x -
                               zVector.x * Temp.yVector.y +
                               wVector.x * Temp.yVector.z);
    Result.yVector.z = -rDet * (xVector.x * Temp.zVector.x -
                                yVector.x * Temp.zVector.y +
                                wVector.x * Temp.zVector.z);
    Result.yVector.w = rDet * (xVector.x * Temp.wVector.x -
                               yVector.x * Temp.wVector.y +
                               zVector.x * Temp.wVector.z);
    Result.zVector.x = rDet * (yVector.x * (zVector.y * wVector.w - zVector.w * wVector.y) -
                               zVector.x * (yVector.y * wVector.w - yVector.w * wVector.y) +
                               wVector.x * (yVector.y * zVector.w - yVector.w * zVector.y));
    Result.zVector.y = -rDet * (xVector.x * (zVector.y * wVector.w - zVector.w * wVector.y) -
                                zVector.x * (xVector.y * wVector.w - xVector.w * wVector.y) +
                                wVector.x * (xVector.y * zVector.w - xVector.w * zVector.y));
    Result.zVector.z = rDet * (xVector.x * (yVector.y * wVector.w - yVector.w * wVector.y) -
                               yVector.x * (xVector.y * wVector.w - xVector.w * wVector.y) +
                               wVector.x * (xVector.y * yVector.w - xVector.w * yVector.y));
    Result.zVector.w = -rDet * (xVector.x * (yVector.y * zVector.w - yVector.w * zVector.y) -
                                yVector.x * (xVector.y * zVector.w - xVector.w * zVector.y) +
                                zVector.x * (xVector.y * yVector.w - xVector.w * yVector.y));
    Result.wVector.x = -rDet * (yVector.x * (zVector.y * wVector.z - zVector.z * wVector.y) -
                                zVector.x * (yVector.y * wVector.z - yVector.z * wVector.y) +
                                wVector.x * (yVector.y * zVector.z - yVector.z * zVector.y));
    Result.wVector.y = rDet * (xVector.x * (zVector.y * wVector.z - zVector.z * wVector.y) -
                               zVector.x * (xVector.y * wVector.z - xVector.z * wVector.y) +
                               wVector.x * (xVector.y * zVector.z - xVector.z * zVector.y));
    Result.wVector.z = -rDet * (xVector.x * (yVector.y * xVector.z - yVector.z * wVector.y) -
                                yVector.x * (xVector.y * wVector.z - xVector.z * wVector.y) +
                                wVector.x * (xVector.y * yVector.z - xVector.z * yVector.y));
    Result.wVector.w = rDet * (xVector.x * (yVector.y * zVector.z - yVector.z * zVector.y) -
                               yVector.x * (xVector.y * zVector.z - xVector.z * zVector.y) +
                               zVector.x * (xVector.y * yVector.z - xVector.z * yVector.y));
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

  OrthoMatrix::OrthoMatrix(float Left,
                           float Right,
                           float Top,
                           float Bottom,
                           float Near,
                           float Far,
                           bool Variation) {
    xVector = Vector4(2 / (Right - Left), 0, 0, -((Right + Left) / (Right - Left)));
    yVector = Vector4(0, 2 / (Top - Bottom), 0, -((Top + Bottom) / (Top - Bottom)));
    zVector = Variation ? Vector4(0, 0, 1 / (Far - Near), -(Near / (Far - Near))) : 
                          Vector4(0, 0, 2 / (Far - Near), -((Far + Near) / (Far - Near)));   
    wVector = Vector4(0, 0, 0, 1);
  }
}