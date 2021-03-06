/*****************************************************************************/
/**
* @file    ovVector2.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/02/05
* @brief   Structure for floating points in 2D space
*/
/*****************************************************************************/
#pragma once

#ifndef _INC_VECTOR2_H
# define _INC_VECTOR2_H
#endif

#include "ovMath.h"
#include "ovVector2I.h"

namespace ovEngineSDK {
  class Vector3;

  class OV_UTILITY_EXPORT Vector2
  {
   public:
    /**
    * @brief     Default constructor (no initialization).
    */
    Vector2() = default;

    /**
    * @brief     Create and initialize a new instance with the specified coordinates.
    * @param[in] InX X-coordinate.
    * @param[in] InY Y-coordinate.
    */
    Vector2(float InX, float InY);

    /**
    * @brief     Constructs a Vector2 from a Vector2I
    * @param[in] InPos Integer point used to set the new vector
    */
    explicit Vector2(Vector2I In);

    /**
     * @brief    Get the result of addition on this vector.
     * @param    Other The other vector to add to this.
     * @return   A new combined vector.
     */
    Vector2
    operator+(const Vector2 & Other) const;

    /**
    * @brief     Get the result of subtraction from this vector.
    * @param[in] Other The other vector to subtract from this.
    * @return    A new subtracted vector.
    */
    Vector2
    operator-(const Vector2 & Other) const;

    /**
    * @brief    Get the result of division on this vector.
    * @param[in]Other The other vector to subtract from this.
    * @return   A new subtracted vector.
    */
    Vector2
    operator/(const Vector2 & Other) const;

    /**
     * @brief Add another vector component-wise to this vector.
     * @param Other The vector to add to this vector.
     * @return Reference to this vector after addition.
     */
    Vector2&
    operator+=(const Vector2 & Other);

    /**
     * @brief Subtract another vector component-wise from this vector.
     * @param Other The vector to subtract from this vector.
     * @return Reference to this vector after subtraction.
     */
    Vector2&
    operator-=(const Vector2 & Other);

    /**
     * @brief Divide this vector component-wise by another vector.
     * @param Other The vector to divide with.
     * @return Reference to this vector after division.
     */
    Vector2&
    operator/=(const Vector2 & Other);

    /**
    * @brief     Compare two vectors for equality.
    * @param[in] Other Other vector to compare.
    * @return    true if vectors are equal, otherwise false.
    */
    bool
    operator==(const Vector2 & Other) const;

    /**
    * @brief     Compare two vectors for inequality.
    * @param[in] Other Other vector to compare.
    * @return    true if vectors aren't equal, otherwise false.
    */
    bool
    operator!=(const Vector2 & Other) const;

    /**
     * @brief Get the result of scaling on this vector.
     * @param Scale What to multiply the vector by.
     * @return A new scaled vector.
     */
    Vector2
    operator*(float Scale) const;

    /**
     * @brief Get the result of division on this vector.
     * @param Divisor What to divide the vector by.
     * @return A new divided vector.
     */
    Vector2
    operator/(float Divisor) const;

    /**
     * @brief Scale this vector.
     * @param Scale What to multiply the vector by.
     * @return Reference to this vector after multiplication.
     */
    Vector2&
    operator*=(float Scale);

    /**
     * @brief Divide this vector by a scalar.
     * @param Divisor What to divide the vector by.
     * @return Reference to this vector after division.
     */
    Vector2&
    operator/=(float Divisor);

    /**
     * @brief Calculates the dot (scalar) product of this vector with another.
     */
    float
    dot(const Vector2 & vec) const;

    /**
    * @brief      Calculates the lenght of the vector
    * @return     length of the vector
    */
    float
    magnitude();

   public:
    float x;
    float y;

    static const Vector2 ZERO;
  };
}