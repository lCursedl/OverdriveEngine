#include <gtest/gtest.h>
#include <DirectXMath.h>

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

  EXPECT_EQ(Math::ceil(1.000001f), 2);
  EXPECT_EQ(Math::ceil(1.00000001f), 1);
  EXPECT_EQ(Math::ceilFloat(1.000001f), 2.0f);
  EXPECT_EQ(Math::ceilFloat(1.00000001f), 1.0f);

  EXPECT_EQ(Math::floorFloat(1.9999999f), 1.0f);
  EXPECT_EQ(Math::floorFloat(1.99999999f), 2.0f);

  EXPECT_FLOAT_EQ(Math::sqrt(Math::square(5.f)), 5.f);
  EXPECT_FLOAT_EQ(Math::invSqrt(5.f) * 5.f, Math::sqrt(5.f));

  EXPECT_NEAR(Math::fmod(18.5f, 4.2f), 1.7f, Math::SMALL_NUMBER);
  EXPECT_NEAR(Math::exp(5), 148.413159f, Math::SMALL_NUMBER);

  EXPECT_FLOAT_EQ(Math::sqrt(Math::square(5.f)), 5.f);
  EXPECT_FLOAT_EQ(Math::invSqrt(5.f) * 5.f, Math::sqrt(5.f));
  EXPECT_NEAR(Math::invSqrt(5.f) * 5.f, Math::sqrt(5.f), 0.00001f);
}

TEST(ovUtility, Math_Trigonometric) {
  EXPECT_FLOAT_EQ(Math::sin(0.f), 0.f);
  EXPECT_FLOAT_EQ(Math::cos(0.f), 1.f);
  EXPECT_FLOAT_EQ(Math::tan(Math::HALF_PI * 0.5f), 1.f);
  
  EXPECT_NEAR(Math::acos(0.f).toRadians(), 1.57079633f, 0);
  EXPECT_NEAR(Math::acos(0.f).toRadians(), 1.57079600f, Math::SMALL_NUMBER);

  EXPECT_NEAR(Math::asin(1.f).toRadians(), 1.57079633f, 0);
  EXPECT_NEAR(Math::asin(1.f).toRadians(), 1.57079600f, Math::SMALL_NUMBER);

  EXPECT_NEAR(Math::atan(1.f).toRadians(), 0.785398185f, 0);
  EXPECT_NEAR(Math::atan(1.f).toRadians(), 0.785398100f, Math::SMALL_NUMBER);
  
  EXPECT_FLOAT_EQ(Math::atan2(1.f, 1.f).toRadians(), Math::HALF_PI * 0.5f);
  EXPECT_FLOAT_EQ(Degree(Math::atan2(1.f, 1.f)).toDegrees(), 45.0f);

  EXPECT_FLOAT_EQ(Math::atan2(-1.f, 1.f).toRadians(), -(Math::HALF_PI * 0.5f));
  EXPECT_FLOAT_EQ(Degree(Math::atan2(-1.f, 1.f)).toDegrees(), -45.0f);

  EXPECT_FLOAT_EQ(Math::atan2(1.f, -1.f).toRadians(), Math::PI * 0.75f);
  EXPECT_FLOAT_EQ(Degree(Math::atan2(1.f, -1.f)).toDegrees(), 135.0f);

  EXPECT_FLOAT_EQ(Math::atan2(-1.f, -1.f).toRadians(), -(Math::PI * 0.75f));
  EXPECT_FLOAT_EQ(Degree(Math::atan2(-1.f, -1.f)).toDegrees(), -135.0f);
}

TEST(ovUtility, Math_Vector2I) {
  Vector2I vector0(5, 5);
  Vector2I vector1(3, 4);

  int32 value = vector1.dot(vector0);
  EXPECT_TRUE(35 == value);
}

TEST(ovUtility, Matrix4_LookAt) {
  DirectX::XMVECTOR EyePos = DirectX::XMVectorSet(0.0f, -100.0f, 0.0f, 1.0f);
  DirectX::XMVECTOR LookAtpos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
  DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
  DirectX::XMMATRIX lookAt = DirectX::XMMatrixLookAtLH(EyePos,
                                                       LookAtpos,
                                                       Up);
  Vector3 myEyePos(0.f, -100.f, 0.f);
  Vector3 myLookAtPos(0.f, 0.f, 0.f);
  Vector3 myUp(0.f, 0.f, 1.f);
  LookAtMatrix myLookAt(myEyePos, myLookAtPos, myUp);

  EXPECT_FLOAT_EQ(myLookAt.xVector.x, DirectX::XMVectorGetByIndex(lookAt.r[0], 0));
  EXPECT_FLOAT_EQ(myLookAt.xVector.y, DirectX::XMVectorGetByIndex(lookAt.r[0], 1));
  EXPECT_FLOAT_EQ(myLookAt.xVector.z, DirectX::XMVectorGetByIndex(lookAt.r[0], 2));
  EXPECT_FLOAT_EQ(myLookAt.xVector.w, DirectX::XMVectorGetByIndex(lookAt.r[0], 3));

  EXPECT_FLOAT_EQ(myLookAt.yVector.x, DirectX::XMVectorGetByIndex(lookAt.r[1], 0));
  EXPECT_FLOAT_EQ(myLookAt.yVector.y, DirectX::XMVectorGetByIndex(lookAt.r[1], 1));
  EXPECT_FLOAT_EQ(myLookAt.yVector.z, DirectX::XMVectorGetByIndex(lookAt.r[1], 2));
  EXPECT_FLOAT_EQ(myLookAt.yVector.w, DirectX::XMVectorGetByIndex(lookAt.r[1], 3));

  EXPECT_FLOAT_EQ(myLookAt.zVector.x, DirectX::XMVectorGetByIndex(lookAt.r[2], 0));
  EXPECT_FLOAT_EQ(myLookAt.zVector.y, DirectX::XMVectorGetByIndex(lookAt.r[2], 1));
  EXPECT_FLOAT_EQ(myLookAt.zVector.z, DirectX::XMVectorGetByIndex(lookAt.r[2], 2));
  EXPECT_FLOAT_EQ(myLookAt.zVector.w, DirectX::XMVectorGetByIndex(lookAt.r[2], 3));

  EXPECT_FLOAT_EQ(myLookAt.wVector.x, DirectX::XMVectorGetByIndex(lookAt.r[3], 0));
  EXPECT_FLOAT_EQ(myLookAt.wVector.y, DirectX::XMVectorGetByIndex(lookAt.r[3], 1));
  EXPECT_FLOAT_EQ(myLookAt.wVector.z, DirectX::XMVectorGetByIndex(lookAt.r[3], 2));
  EXPECT_FLOAT_EQ(myLookAt.wVector.w, DirectX::XMVectorGetByIndex(lookAt.r[3], 3));
}

TEST(ovUtility, Matrix4_Perspective) {
  float FOVAngleY = Degree(60).toRadians();
  float AspectRatio = 1920.f / 1080.f;
  float Near = 1.f;
  float Far = 1000.f;

  DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(FOVAngleY,
                                                                   AspectRatio,
                                                                   Near,
                                                                   Far);

  PerspectiveMatrix myProjection(FOVAngleY * 0.5f, 1920.f, 1080.f, Near, Far);

  EXPECT_FLOAT_EQ(myProjection.yVector.y / myProjection.xVector.x,
                  DirectX::XMVectorGetByIndex(projection.r[1], 1) /
                  DirectX::XMVectorGetByIndex(projection.r[0], 0));
  EXPECT_FLOAT_EQ(myProjection.zVector.w, DirectX::XMVectorGetByIndex(projection.r[2], 3));
  EXPECT_FLOAT_EQ(myProjection.wVector.z, DirectX::XMVectorGetByIndex(projection.r[3], 2));
}