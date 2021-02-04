#pragma once

#ifndef _INC_VECTOR2I_H_
# define _INC_VECTOR2I_H_
#endif

#include "ovPlatformDefines.h"
#include "ovPlatformTypes.h"

namespace ovEngineSDK {
  /**
  * @brief Structure for integer points in 2-d space.
  */
	class OV_UTILITY_EXPORT Vector2I
	{
	 public:
    /**
    * @brief Default constructor (no initialization).
    */
    Vector2I() = default;

    /**
    * @brief Create and initialize a new instance with the specified coordinates.
    * @param InX X-coordinate.
    * @param InY Y-coordinate.
    */
    Vector2I(int32 InX, int32 InY);

    /**
    * @brief Compare two vectors for equality.
    * @param Other Other vector to compare.
    * @return true if vectors are equal, otherwise false.
    */
    bool
    operator==(const Vector2I& Other) const;

    /**
    * @brief Compare two vectors for inequality.
    * @param Other Other vector to compare.
    * @return true if vectors aren't equal, otherwise false.
    */
    bool
    operator!=(const Vector2I& Other) const;

    /**
     * @brief Add another vector component-wise to this vector.
     * @param Other The vector to add to this vector.
     * @return Reference to this vector after addition.
     */
    Vector2I&
      operator+=(const Vector2I& Other);

    /**
     * @brief Subtract another vector component-wise from this vector.
     * @param Other The vector to subtract from this vector.
     * @return Reference to this vector after subtraction.
     */
    Vector2I&
      operator-=(const Vector2I& Other);

    /**
     * @brief Scale this vector.
     * @param Scale What to multiply the vector by.
     * @return Reference to this vector after multiplication.
     */
    Vector2I&
      operator*=(int32 Scale);

    /**
     * @brief Divide this vector by a scalar.
     * @param Divisor What to divide the vector by.
     * @return Reference to this vector after division.
     */
    Vector2I&
      operator/=(int32 Divisor);

    /**
     * @brief Divide this vector component-wise by another vector.
     * @param Other The vector to divide with.
     * @return Reference to this vector after division.
     */
    Vector2I&
      operator/=(const Vector2I& Other);

    /**
     * @brief Get the result of scaling on this vector.
     * @param Scale What to multiply the vector by.
     * @return A new scaled vector.
     */
    Vector2I
      operator*(int32 Scale) const;

    /**
     * @brief Get the result of division on this vector.
     * @param Divisor What to divide the vector by.
     * @return A new divided vector.
     */
    Vector2I
      operator/(int32 Divisor) const;

    /**
     * @brief Get the result of addition on this vector.
     * @param Other The other vector to add to this.
     * @return A new combined vector.
     */
    Vector2I
      operator+(const Vector2I& Other) const;

    /**
     * @brief Get the result of subtraction from this vector.
     * @param Other The other vector to subtract from this.
     * @return A new subtracted vector.
     */
    Vector2I
      operator-(const Vector2I& Other) const;

    /**
     * @brief Get the result of division on this vector.
     * @param Other The other vector to subtract from this.
     * @return A new subtracted vector.
     */
    Vector2I
      operator/(const Vector2I& Other) const;
   private:
	};
}