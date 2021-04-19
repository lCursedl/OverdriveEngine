/*****************************************************************************/
/**
* @file    ovVector3.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/02/05
* @brief   Structure for float points in 3D space
*/
/*****************************************************************************/
#pragma once

#ifndef _INC_VECTOR3_H_
# define _INC_VECTOR3_H_
#endif // !_INC_VECTOR3_H_

#include "ovVector3I.h"
#include "ovVector2.h"

namespace ovEngineSDK {
  class OV_UTILITY_EXPORT Vector3
  {
   public:
     /**
       * @brief     Default constructor (no initialization).
       */
     Vector3() = default;

     /**
     * @brief     Create and initialize a new instance with the specified coordinates.
     * @param[in] InX X-coordinate.
     * @param[in] InY Y-coordinate.
     */
     Vector3(float InX, float InY, float Z);

     /**
     * @brief     Create and initialize a new instance from a Vector2 and an int
     * @param[in] Vec Vector2 to take values from
     * @param[in] Value Value for Z
     */
     Vector3(Vector2 & Vec, float Value);

     /**
     * @brief     Constructs a Vector3 from a Vector3I
     * @param[in] In Integer point used to set the new vector
     */
     explicit Vector3(Vector3I In);

     /**
      * @brief    Get the result of addition on this vector.
      * @param    Other The other vector to add to this.
      * @return   A new combined vector.
      */
     Vector3
     operator+(const Vector3 & Other) const;

     /**
     * @brief     Get the result of subtraction from this vector.
     * @param[in] Other The other vector to subtract from this.
     * @return    A new subtracted vector.
     */
     Vector3
     operator-(const Vector3 & Other) const;

     /**
     * @brief    Get the result of division on this vector.
     * @param[in]Other The other vector to subtract from this.
     * @return   A new subtracted vector.
     */
     Vector3
     operator/(const Vector3 & Other) const;

     /**
      * @brief Add another vector component-wise to this vector.
      * @param Other The vector to add to this vector.
      * @return Reference to this vector after addition.
      */
     Vector3&
     operator+=(const Vector3 & Other);

     /**
      * @brief Subtract another vector component-wise from this vector.
      * @param Other The vector to subtract from this vector.
      * @return Reference to this vector after subtraction.
      */
     Vector3&
     operator-=(const Vector3 & Other);

     /**
      * @brief Divide this vector component-wise by another vector.
      * @param Other The vector to divide with.
      * @return Reference to this vector after division.
      */
     Vector3&
     operator/=(const Vector3 & Other);

     /**
     * @brief     Compare two vectors for equality.
     * @param[in] Other Other vector to compare.
     * @return    true if vectors are equal, otherwise false.
     */
     bool
     operator==(const Vector3 & Other) const;

     /**
     * @brief     Compare two vectors for inequality.
     * @param[in] Other Other vector to compare.
     * @return    true if vectors aren't equal, otherwise false.
     */
     bool
     operator!=(const Vector3 & Other) const;

     /**
      * @brief Get the result of scaling on this vector.
      * @param Scale What to multiply the vector by.
      * @return a new scaled vector.
      */
     Vector3
     operator*(float Scale) const;

     /**
      * @brief Get the result of division on this vector.
      * @param Divisor What to divide the vector by.
      * @return a new divided vector.
      */
     Vector3
     operator/(float Divisor) const;

     /**
      * @brief Scale this vector.
      * @param Scale What to multiply the vector by.
      * @return reference to this vector after multiplication.
      */
     Vector3&
     operator*=(float Scale);

     /**
      * @brief Divide this vector by a scalar.
      * @param Divisor What to divide the vector by.
      * @return reference to this vector after division.
      */
     Vector3&
     operator/=(float Divisor);

     Vector3
     operator^(const Vector3& v) const;

     Vector3
     operator-() const;

     /**
      * @brief     Calculates the dot (scalar) product of this vector with another.
      * @param[in] Other Vector2 to realize dot product
      * @return    the range of similarity from one vector to the other
      */
     float
     dot(const Vector3 & vec) const;

     Vector3
     cross(const Vector3& a, const Vector3& b);

     /**
     * @brief      Calculates the lenght of the vector
     * @return     length of the vector
     */
     float
     magnitude();

     /**
     * @brief    Normalize this vector if it is large enough, otherwise set to
     *           (0, 0, 0).
     * @param[in]Tolerance Minimum squared lenght of vector for normalization.
     * @return   true if the vector was normalized correctly, otherwise false.
     */
     bool
     normalize(float tolerance = Math::SMALL_NUMBER);

   public:
    float x;
    float y;
    float z;

    static const Vector3 ZERO;
  };
}