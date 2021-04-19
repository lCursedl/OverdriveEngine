/*****************************************************************************/
/**
* @file    ovMath.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/02/05
* @brief   Utility class providing common scalar math operations
*/
/*****************************************************************************/
#pragma once

#include "ovPlatformTypes.h"
#include "ovDegree.h"
#include "ovRadian.h"
#include <cmath>

namespace ovEngineSDK {
  
  class OV_UTILITY_EXPORT Math
  {
   public:
     static int32
     trunc(float F);

     static float
     truncFloat(float F);

     static float
     floorFloat(float F);

     static int32
     round(float F);

     static float
     roundFloat(float F);

     static int32
     ceil(float F);

     static float
     ceilFloat(float F);

     static float
     fractional(float Value);

     static float
     fmod(float X, float Y);

     static float
     pow(float Base, float Exponent);

     static float
     exp(float Value);

     static float
     logE(float Value);

     static float
     log2(float Value);

     static float
     logX(float Base, float Value);

     static float
     sqrt(float Value);

     static Radian
     sqrt(const Radian& Value);

     static Degree
     sqrt(const Degree& Value);

     static float
     invSqrt(float F);

     static float
     cos(float Value);

     static float
     sin(float Value);

     static float
     tan(float Value);

     static float
     cos(const Radian& Value);

     static float
     sin(const Radian& Value);

     static float
     tan(const Radian& Value);

     static float
     cosh(float Value);

     static float
     sinh(float Value);

     static float
     tanh(float Value);

     static Radian
     acos(float Value);

     static Radian
     asin(float Value);

     static Radian
     atan(float Value);

     static Radian
     atan2(float Y, float X);

     template<class T>
     static T
     square(const T Value) {
       return Value * Value;
     }

     static float
     cot(float Value);

     /**
     * @brief     Generic Lineal Interpolation function
     * @param[in] x0 Starting value
     * @param[in] x1 Ending value
     * @param[in] Alpha Time coefficient in range from 0 to 1
     * @return value between x0 and x1 related to alpha
     */
     template<class T, class U>
     static T
     lerp(const T& A, const T& B, const U& Alpha) {
       return static_cast<T>(A + Alpha * (B - A));
     }

     public:

     static const float PI;
     static const float INV_PI;
     static const float HALF_PI;
     
     static const float DEG2RAD;
     static const float RAD2DEG;
     static const float LOG2;

     static const float SMALL_NUMBER;
     static const float BIG_NUMBER;
  };
}