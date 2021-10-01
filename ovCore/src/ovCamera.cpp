#include <ovCamera.h>

namespace ovEngineSDK {
  
  Camera::Camera() {
    m_componentID = COMPONENT_TYPE::kCAMERA;
  }
  void
  Camera::init(const Vector3 Position,
               const Vector3 LookAt,
               const Vector3 Up,
               float Step,
               const Matrix4 projection) {
    m_viewMat = LookAtMatrix(Position, LookAt, Up);
    m_projMat = projection;
    m_step = Step;
  }

  void
  Camera::setProjection(const Matrix4 projection) {
    m_projMat = projection;
  }

  void
  Camera::move(float delta) {
    Vector3 position = Vector3(m_viewMat.wVector.x,
                               m_viewMat.wVector.y,
                               m_viewMat.wVector.z);
    //Check forward input
    position = m_forward ? position + (Vector3(m_viewMat.zVector.x,
                                               m_viewMat.zVector.y,
                                               m_viewMat.zVector.z) * (-m_step * delta))
                                               : position;
    //Check backwards input
    position = m_back    ? position + (Vector3(m_viewMat.zVector.x,
                                               m_viewMat.zVector.y,
                                               m_viewMat.zVector.z) * (m_step * delta))
                                               : position;
    //Check left input
    position = m_left    ? position + (Vector3(m_viewMat.xVector.x,
                                               m_viewMat.xVector.y,
                                               m_viewMat.xVector.z) * (m_step * delta))
                                               : position;
    //Check right input
    position = m_right   ? position + (Vector3(m_viewMat.xVector.x,
                                               m_viewMat.xVector.y,
                                               m_viewMat.xVector.z) * (-m_step * delta))
                                               : position;
    //Check up input
    position = m_up      ? position + (Vector3(m_viewMat.yVector.x,
                                               m_viewMat.yVector.y,
                                               m_viewMat.yVector.z) * (m_step * delta))
                                               : position;
    //Check down input
    position = m_down    ? position + (Vector3(m_viewMat.yVector.x,
                                               m_viewMat.yVector.y,
                                               m_viewMat.yVector.z) * (-m_step * delta))
                                               : position;
    //Update position
    m_viewMat.wVector.x = position.x;
    m_viewMat.wVector.y = position.y;
    m_viewMat.wVector.z = position.z;
  }

  void
  Camera::rotate(const Vector2& direction, float delta) {
    rotateRight(direction, delta);
    rotateUp(direction, delta);
  }

  void
  Camera::update(float delta) {
    move(delta);
  }

  void
  Camera::render() {}

  void
  Camera::setForward(bool state) {
    m_forward = state;
  }

  void
  Camera::setBackward(bool state) {
    m_back = state;
  }

  void
  Camera::setLeft(bool state) {
    m_left = state;
  }

  void
  Camera::setRight(bool state) {
    m_right = state;
  }

  void
  Camera::setUp(bool state) {
    m_up = state;
  }

  void
  Camera::setDown(bool state) {
    m_down = state;
  }

  void
  Camera::setRotateRight(bool state) {
    m_rRight = state;
  }

  void
  Camera::setRotateLeft(bool state) {
    m_rLeft = state;
  }

  const Matrix4
  Camera::getView() {
    return m_viewMat;
  }

  const Matrix4
  Camera::getProjection() {
    return m_projMat;
  }

  void
  Camera::roll(float delta) {
    if (m_rLeft) {
      rotateFront(delta);
    }
    if (m_rRight) {
      rotateFront(-delta);
    }
  }

  void
  Camera::rotateRight(const Vector2& direction, float delta) {
    
      float camCos = Math::cos(direction.x * m_sensibility * delta);
      float camSin = Math::sin(direction.x * m_sensibility * delta);

      Matrix4 rotX = Matrix4(camCos,  0.f, camSin, 0.f,
                             0.f,     1.f,    0.f, 0.f,
                             -camSin, 0.f, camCos, 0.f,
                             0.f,     0.f,    0.f, 1.f);
      m_viewMat *= rotX;
  }

  void
  Camera::rotateUp(const Vector2& direction, float delta) {
    
      float camCos = Math::cos(direction.y * m_sensibility * delta);
      float camSin = Math::sin(direction.y * m_sensibility * delta);

      Matrix4 rotY = Matrix4(1.f,     0.f,    0.f, 0.f,
                             0.f,  camCos, -camSin, 0.f,
                             0.f, camSin, camCos, 0.f,
                             0.f,     0.f,    0.f, 1.f);
      m_viewMat *= rotY;
  }

  void
  Camera::rotateFront(float delta) {
    float camCos = Math::cos(m_sensibility * delta);
    float camSin = Math::sin(m_sensibility * delta);

    Matrix4 rotZ = Matrix4(camCos, -camSin, 0.f, 0.f,
                          camSin, camCos, 0.f, 0.f,
                              0.f,    0.f, 1.f, 0.f,
                              0.f,    0.f, 0.f, 1.f);
    m_viewMat *= rotZ;
  }
}
