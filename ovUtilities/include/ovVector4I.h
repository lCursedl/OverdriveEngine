/*****************************************************************************/
/**
* @file    ovVector4I.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/02/05
* @brief   Structure for integer points in 4D space.
*/
/*****************************************************************************/
#pragma once

#ifndef _INC_VECTOR4I_H
#define _INC_VECTOR4I_H
#endif // !_INC_VECTOR4_H

#include "ovPrerequisitesUtil.h"
#include "ovMath.h"
#include "ovVector2I.h"
#include "ovVector3I.h"

namespace ovEngineSDK {
  class OV_UTILITY_EXPORT Vector4I
  {
   public:
     /**
     * @brief     Default constructor (no initialization).
     */
     Vector4I() = default;

     /**
     * @brief     Create and initialize a new instance with the specified coordinates.
     * @param[in] InX X-coordinate.
     * @param[in] InY Y-coordinate.
     * @param[in] InZ Z-coordinate.
     * @param[in] InW W-coordinate.
     */
     Vector4I(int32 InX, int32 InY, int32 InZ, int32 InW);

     /**
     * @brief     Create and initialize a new instance from a Vector2I and two ints
     * @param[in] Vec Vector2I to take values from.
     * @param[in] Z   Value for z.
     * @param[in] W   Value for w.
     */
     Vector4I(Vector2I & Vec, int32 Z, int32 W);

     /**
     * @brief     Create and initialize a new instance from a Vector3I and an int
     * @param[in] Vec Vector3I to take values from
     * @param[in] W Value for w
     */
     Vector4I(Vector3I& Vec, int32 W);

     /**
      * @brief    Get the result of addition on this vector.
      * @param    Other The other vector to add to this.
      * @return   A new combined vector.
      */
     Vector4I
     operator+(const Vector4I & Other) const;

     /**
     * @brief     Get the result of subtraction from this vector.
     * @param[in] Other The other vector to subtract from this.
     * @return    A new subtracted vector.
     */
     Vector4I
     operator-(const Vector4I & Other) const;

     /**
     * @brief    Get the result of division on this vector.
     * @param[in]Other The other vector to subtract from this.
     * @return   A new subtracted vector.
     */
     Vector4I
     operator/(const Vector4I & Other) const;

     /**
      * @brief Add another vector component-wise to this vector.
      * @param Other The vector to add to this vector.
      * @return Reference to this vector after addition.
      */
     Vector4I&
     operator+=(const Vector4I & Other);

     /**
      * @brief Subtract another vector component-wise from this vector.
      * @param Other The vector to subtract from this vector.
      * @return Reference to this vector after subtraction.
      */
     Vector4I&
     operator-=(const Vector4I & Other);

     /**
      * @brief Divide this vector component-wise by another vector.
      * @param Other The vector to divide with.
      * @return Reference to this vector after division.
      */
     Vector4I&
     operator/=(const Vector4I & Other);

     /**
     * @brief     Compare two vectors for equality.
     * @param[in] Other Other vector to compare.
     * @return    true if vectors are equal, otherwise false.
     */
     bool
     operator==(const Vector4I & Other) const;

     /**
     * @brief     Compare two vectors for inequality.
     * @param[in] Other Other vector to compare.
     * @return    true if vectors aren't equal, otherwise false.
     */
     bool
     operator!=(const Vector4I & Other) const;

     /**
      * @brief Get the result of scaling on this vector.
      * @param Scale What to multiply the vector by.
      * @return a new scaled vector.
      */
     Vector4I
     operator*(int32 Scale) const;

     /**
      * @brief Get the result of division on this vector.
      * @param Divisor What to divide the vector by.
      * @return a new divided vector.
      */
     Vector4I
     operator/(int32 Divisor) const;

     /**
      * @brief Scale this vector.
      * @param Scale What to multiply the vector by.
      * @return reference to this vector after multiplication.
      */
     Vector4I&
     operator*=(int32 Scale);

     /**
      * @brief Divide this vector by a scalar.
      * @param Divisor What to divide the vector by.
      * @return reference to this vector after division.
      */
     Vector4I&
     operator/=(int32 Divisor);

     /**
      * @brief     Calculates the dot (scalar) product of this vector with another.
      * @param[in] Other Vector4I to realize dot product
      * @return    the range of similarity from one vector to the other
      */
     int32
     dot(const Vector4I & vec) const;

     /**
      * @brief     Calculates the cross (product) vector of this vector with another.
      * @param[in] Other Vector4I to realize dot product
      * @return    product vector
      */
     Vector4I
     cross(const Vector4I & vec) const;

     /**
     * @brief      Calculates the lenght of the vector
     * @return     length of the vector
     */
     int32
     magnitude();
   public:
    int32 x;
    int32 y;
    int32 z;
    int32 w;

    static const Vector4I ZERO;
  };
}