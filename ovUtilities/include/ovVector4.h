/*****************************************************************************/
/**
* @file    ovVector4.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/02/05
* @brief   Structure for float points in 4D space.
*/
/*****************************************************************************/
#pragma once

#ifndef _INC_VECTOR4_H
#define _INC_VECTOR4_H
#endif // !_INC_VECTOR4_H

#include "ovPrerequisitesUtil.h"
#include "ovMath.h"
#include "ovVector2.h"
#include "ovVector3.h"
#include "ovVector4I.h"

namespace ovEngineSDK {
  class OV_UTILITY_EXPORT Vector4
  {
  public:
    /**
       * @brief     Default constructor (no initialization).
       */
    Vector4() = default;

    /**
    * @brief     Create and initialize a new instance with the specified coordinates.
    * @param[in] InX X-coordinate.
    * @param[in] InY Y-coordinate.
    * @param[in] InZ Z-coordinate.
    * @param[in] InW W-coordinate.
    */
    Vector4(float InX, float InY, float InZ, float InW);

    /**
    * @brief     Create and initialize a new instance from a Vector2I and two ints
    * @param[in] Vec Vector2 to take values from.
    * @param[in] Z   Value for z.
    * @param[in] W   Value for w.
    */
    Vector4(Vector2& Vec, float Z, float W);

    /**
    * @brief     Create and initialize a new instance from a Vector3I and an int
    * @param[in] Vec Vector3 to take values from
    * @param[in] W Value for w
    */
    Vector4(Vector3& Vec, float W);

    /**
    * @brief      Constructs a Vector4 from a Vector4I
    * @param[in]  In Integer point used to set the new vector
    */
    explicit Vector4(Vector4I& Vec);

    /**
     * @brief    Get the result of addition on this vector.
     * @param    Other The other vector to add to this.
     * @return   A new combined vector.
     */
    Vector4
    operator+(const Vector4& Other) const;

    /**
    * @brief     Get the result of subtraction from this vector.
    * @param[in] Other The other vector to subtract from this.
    * @return    A new subtracted vector.
    */
    Vector4
    operator-(const Vector4& Other) const;

    /**
    * @brief    Get the result of division on this vector.
    * @param[in]Other The other vector to subtract from this.
    * @return   A new subtracted vector.
    */
    Vector4
    operator/(const Vector4& Other) const;

    /**
     * @brief Add another vector component-wise to this vector.
     * @param Other The vector to add to this vector.
     * @return Reference to this vector after addition.
     */
    Vector4&
    operator+=(const Vector4& Other);

    /**
     * @brief Subtract another vector component-wise from this vector.
     * @param Other The vector to subtract from this vector.
     * @return Reference to this vector after subtraction.
     */
    Vector4&
    operator-=(const Vector4& Other);

    /**
     * @brief Divide this vector component-wise by another vector.
     * @param Other The vector to divide with.
     * @return Reference to this vector after division.
     */
    Vector4&
    operator/=(const Vector4& Other);

    /**
    * @brief     Compare two vectors for equality.
    * @param[in] Other Other vector to compare.
    * @return    true if vectors are equal, otherwise false.
    */
    bool
    operator==(const Vector4& Other) const;

    /**
    * @brief     Compare two vectors for inequality.
    * @param[in] Other Other vector to compare.
    * @return    true if vectors aren't equal, otherwise false.
    */
    bool
    operator!=(const Vector4& Other) const;

    /**
     * @brief Get the result of scaling on this vector.
     * @param Scale What to multiply the vector by.
     * @return a new scaled vector.
     */
    Vector4
    operator*(float Scale) const;

    /**
     * @brief Get the result of division on this vector.
     * @param Divisor What to divide the vector by.
     * @return a new divided vector.
     */
    Vector4
    operator/(float Divisor) const;

    /**
     * @brief Scale this vector.
     * @param Scale What to multiply the vector by.
     * @return reference to this vector after multiplication.
     */
    Vector4&
    operator*=(float Scale);

    /**
     * @brief Divide this vector by a scalar.
     * @param Divisor What to divide the vector by.
     * @return reference to this vector after division.
     */
    Vector4&
    operator/=(float Divisor);

    /**
     * @brief     Calculates the dot (scalar) product of this vector with another.
     * @param[in] Other Vector4I to realize dot product
     * @return    the range of similarity from one vector to the other
     */
    float
    dot(const Vector4& vec) const;

    /**
     * @brief     Calculates the cross (product) vector of this vector with another.
     * @param[in] Other Vector4I to realize dot product
     * @return    product vector
     */
    Vector4
    cross(const Vector4& vec) const;

    /**
    * @brief      Calculates the lenght of the vector
    * @return     length of the vector
    */
    float
    magnitude();
  public:
    float x;
    float y;
    float z;
    float w;

    static const Vector4 ZERO;
  };
}