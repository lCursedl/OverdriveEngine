/*****************************************************************************/
/**
* @file    ovMatrix4.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/02/05
* @brief   Structure for a 4x4 matrix of floating point values
*/
/*****************************************************************************/
#pragma once

#include "ovPrerequisitesUtil.h"
#include "ovMath.h"
#include "ovVector3.h"
#include "ovVector4.h"

namespace ovEngineSDK {
  
  class Quaternion;

  class OV_UTILITY_EXPORT Matrix4
  {
   public:
    Matrix4() = default;

    Matrix4(float value);

    Matrix4(float _xx, float _xy, float _xz, float _xw,
            float _yx, float _yy, float _yz, float _yw,
            float _zx, float _zy, float _zz, float _zw,
            float _wx, float _wy, float _wz, float _ww);

    Matrix4(const Vector4& VecX,
            const Vector4& VecY,
            const Vector4& VecZ,
            const Vector4& VecW);

    Matrix4(const Vector3& VecX,
            const Vector3& VecY,
            const Vector3& VecZ,
            const Vector3& VecW);

    Matrix4
    operator+(const Matrix4& Mat)const;

    Matrix4
    operator-(const Matrix4& Mat)const;

    Matrix4
    operator*(const Matrix4& Mat) const; 

    Matrix4
    operator+=(const Matrix4& Mat);

    Matrix4
    operator-=(const Matrix4& Mat);

    Matrix4
    operator*=(const Matrix4& Mat);

    bool
    operator==(const Matrix4& Mat);

    bool
    operator!=(const Matrix4& Mat);

    void
    operator=(const Matrix4& Mat);

    Matrix4
    transpose()const;

    static Matrix4
    scale(const Matrix4& Mat, const Vector3& Vec);

    static Matrix4
    fromQuat(const Quaternion& Quat);

    Matrix4
    inverse() const;

   public:
    Vector4 xVector;
    Vector4 yVector;
    Vector4 zVector;
    Vector4 wVector;

    static const Matrix4 IDENTITY;
  };

  class OV_UTILITY_EXPORT LookAtMatrix : public Matrix4
  {
   public:
    LookAtMatrix(const Vector3& EyePosition,
                 const Vector3& LookAtPosition,
                 const Vector3& UpVector);
  };

  class OV_UTILITY_EXPORT PerspectiveMatrix : public Matrix4
  {
    #define Z_PRECISION 0.f
   public:
    PerspectiveMatrix(float FOV,
                      float Width,
                      float Height,
                      float MinZ,
                      float MaxZ);
  };

  class OV_UTILITY_EXPORT OrthoMatrix : public Matrix4
  {
   public:
    OrthoMatrix(float Left,
                float Right,
                float Top,
                float Bottom,
                float Near,
                float Far,
                bool Variation);
  };
}