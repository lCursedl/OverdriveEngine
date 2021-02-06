/*****************************************************************************/
/**
* @file    ovVector3I.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/02/05
* @brief   Structure for integer points in 3D space
*/
/*****************************************************************************/
#pragma once

#ifndef _INC_VECTOR3I_H_
# define _INC_VECTOR3I_H_
#endif // !_INC_VECTOR3I_H_

#include "ovVector2I.h"

namespace ovEngineSDK {
  class OV_UTILITY_EXPORT Vector3I
  {
   public:
     /**
      * @brief     Default constructor (no initialization).
      */
     Vector3I() = default;

     /**
     * @brief     Create and initialize a new instance with the specified coordinates.
     * @param[in] InX X-coordinate.
     * @param[in] InY Y-coordinate.
     */
     Vector3I(int32 InX, int32 InY, int32 Z);

     /**
     * @brief     Create and initialize a new instance from a Vector2I and an int
     * @param[in] Vec Vector2I to take values from
     * @param[in] Value Value for Z
     */
     Vector3I(Vector2I& Vec, int32 Value);

     /**
      * @brief    Get the result of addition on this vector.
      * @param    Other The other vector to add to this.
      * @return   A new combined vector.
      */
     Vector3I
       operator+(const Vector3I & Other) const;

     /**
     * @brief     Get the result of subtraction from this vector.
     * @param[in] Other The other vector to subtract from this.
     * @return    A new subtracted vector.
     */
     Vector3I
       operator-(const Vector3I & Other) const;

     /**
     * @brief    Get the result of division on this vector.
     * @param[in]Other The other vector to subtract from this.
     * @return   A new subtracted vector.
     */
     Vector3I
       operator/(const Vector3I & Other) const;

     /**
      * @brief Add another vector component-wise to this vector.
      * @param Other The vector to add to this vector.
      * @return Reference to this vector after addition.
      */
     Vector3I&
       operator+=(const Vector3I & Other);

     /**
      * @brief Subtract another vector component-wise from this vector.
      * @param Other The vector to subtract from this vector.
      * @return Reference to this vector after subtraction.
      */
     Vector3I&
       operator-=(const Vector3I & Other);

     /**
      * @brief Divide this vector component-wise by another vector.
      * @param Other The vector to divide with.
      * @return Reference to this vector after division.
      */
     Vector3I&
       operator/=(const Vector3I & Other);

     /**
     * @brief     Compare two vectors for equality.
     * @param[in] Other Other vector to compare.
     * @return    true if vectors are equal, otherwise false.
     */
     bool
       operator==(const Vector3I & Other) const;

     /**
     * @brief     Compare two vectors for inequality.
     * @param[in] Other Other vector to compare.
     * @return    true if vectors aren't equal, otherwise false.
     */
     bool
       operator!=(const Vector3I & Other) const;

     /**
      * @brief Get the result of scaling on this vector.
      * @param Scale What to multiply the vector by.
      * @return a new scaled vector.
      */
     Vector3I
       operator*(int32 Scale) const;

     /**
      * @brief Get the result of division on this vector.
      * @param Divisor What to divide the vector by.
      * @return a new divided vector.
      */
     Vector3I
       operator/(int32 Divisor) const;

     /**
      * @brief Scale this vector.
      * @param Scale What to multiply the vector by.
      * @return reference to this vector after multiplication.
      */
     Vector3I&
       operator*=(int32 Scale);

     /**
      * @brief Divide this vector by a scalar.
      * @param Divisor What to divide the vector by.
      * @return reference to this vector after division.
      */
     Vector3I&
       operator/=(int32 Divisor);

     /**
      * @brief     Calculates the dot (scalar) product of this vector with another.
      * @param[in] Other Vector2 to realize dot product
      * @return    the range of similarity from one vector to the other
      */
     int32
       dot(const Vector3I & vec) const;

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

    static const Vector3I ZERO;
  };
}
