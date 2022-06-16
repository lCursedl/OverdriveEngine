#pragma once
#include <ovComponent.h>
#include <ovMatrix4.h>

namespace ovEngineSDK {

  class OV_CORE_EXPORT Camera : public Component
  {
   public:
    Camera();
    ~Camera() = default;

    void init(const Vector3 Position, 
              const Vector3 LookAt,
              const Vector3 Up,
              float Step,
              const Matrix4 projection);

    void setProjection(const Matrix4 projection);
    void move(float delta);
    void rotate(const Vector2& direction, float delta);
    void update(float delta)                                          override;
    void render()                                                     override;

    void setForward(bool state);
    void setBackward(bool state);
    void setLeft(bool state);
    void setRight(bool state);
    void setUp(bool state);
    void setDown(bool state);
    void setRotateRight(bool state);
    void setRotateLeft(bool state);

    const Matrix4 getView();
    const Matrix4 getProjection();

    void roll(float delta);

   private:

    Matrix4 m_viewMat = Matrix4::IDENTITY;
    Matrix4 m_projMat = Matrix4::IDENTITY;
    void rotateRight(const Vector2& direction, float delta);
    void rotateUp(const Vector2& direction, float delta);
    void rotateFront(float delta);

    float m_step = 1.f;
    float m_sensibility = 0.001f;

    bool m_forward = false;
    bool m_back = false;
    bool m_left = false;
    bool m_right = false;
    bool m_up = false;
    bool m_down = false;
    bool m_rLeft = false;
    bool m_rRight = false;
  };
}
