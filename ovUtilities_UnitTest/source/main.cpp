#include <gtest/gtest.h>

#include <ovPrerequisitesUtil.h>
#include <ovMath.h>
#include <ovVector2.h>
#include <ovVector2I.h>
#include <ovVector3.h>
#include <ovVector3I.h>
#include <ovVector4I.h>
#include <ovVector4.h>
#include <ovMatrix4.h>

using namespace ovEngineSDK;

int
main(int argc, char** argv) {
  printf("Running main() from %s\n", __FILE__);
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(ovUtility, Basic_Type_Size) {
  EXPECT_EQ(sizeof(unsigned char), 1U);
  EXPECT_EQ(sizeof(uint8), 1U);
  EXPECT_EQ(sizeof(uint16), 2U);
  EXPECT_EQ(sizeof(uint32), 4U);
  EXPECT_EQ(sizeof(uint64), 8U);

  EXPECT_EQ(sizeof(char), 1);
  EXPECT_EQ(sizeof(int8), 1);
  EXPECT_EQ(sizeof(int16), 2);
  EXPECT_EQ(sizeof(int32), 4);
  EXPECT_EQ(sizeof(int64), 8);

  EXPECT_EQ(sizeof(float), 4);
  EXPECT_EQ(sizeof(double), 8);

  EXPECT_FLOAT_EQ(Math::PI, 3.141592f);
}

TEST(ovUtility, Math_Basic) {
  EXPECT_EQ(Math::ceil(1.000001f), 2);
  EXPECT_EQ(Math::ceil(1.00000001f), 1);
  EXPECT_EQ(Math::ceilFloat(1.000001f), 2.0f);
  EXPECT_EQ(Math::ceilFloat(1.00000001f), 1.0f);

  EXPECT_EQ(Math::floorFloat(1.9999999f), 1.0f);
  EXPECT_EQ(Math::floorFloat(1.99999999f), 2.0f);

  EXPECT_FLOAT_EQ(Math::sqrt(Math::square(5.f)), 5.f);
  EXPECT_FLOAT_EQ(Math::invSqrt(5.f) * 5.f, Math::sqrt(5.f));
}

TEST(ovUtility, Math_Trigonometric) {
  EXPECT_FLOAT_EQ(Math::sin(0.0f), 0.0f);
  EXPECT_FLOAT_EQ(Math::cos(0.0f), 1.0f);
  EXPECT_FLOAT_EQ(Math::tan(Math::HALF_PI * 0.5f), 1.0f);

  EXPECT_NEAR(Math::acos(0.0f).toRadians(), 1.57079633f, 0);
  EXPECT_NEAR(Math::asin(1.0f).toRadians(), 1.57079633f, 0);
  EXPECT_NEAR(Math::atan(1.0f).toRadians(), 0.785398185f, 0);
}

TEST(ovUtility, Math_Vector2I) {
  Vector2I vector0(5, 5);
  Vector2I vector2(3, 4);

  int32 value = vector2.dot(vector0);
  EXPECT_TRUE(35 == value);
}