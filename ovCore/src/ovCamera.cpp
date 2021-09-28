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
    Vector3 position = Vector3(-m_viewMat.wVector.x,
                               -m_viewMat.wVector.y,
                               -m_viewMat.wVector.z);
    //Check forward input
    position = m_forward ? position + (Vector3(m_viewMat.xVector.z,
                                               m_viewMat.yVector.z,
                                               m_viewMat.zVector.z) * (m_step * delta))
                                               : position;
    //Check backwards input
    position = m_back    ? position + (Vector3(m_viewMat.xVector.z,
                                               m_viewMat.yVector.z,
                                               m_viewMat.zVector.z) * (-m_step * delta))
                                               : position;
    //Check left input
    position = m_left    ? position + (Vector3(m_viewMat.xVector.x,
                                               m_viewMat.yVector.x,
                                               m_viewMat.zVector.x) * (-m_step * delta))
                                               : position;
    //Check right input
    position = m_right   ? position + (Vector3(m_viewMat.xVector.x,
                                               m_viewMat.yVector.x,
                                               m_viewMat.zVector.x) * (m_step * delta))
                                               : position;
    //Check up input
    position = m_up      ? position + (Vector3(m_viewMat.xVector.y,
                                               m_viewMat.yVector.y,
                                               m_viewMat.zVector.y) * (-m_step * delta))
                                               : position;
    //Check down input
    position = m_down    ? position + (Vector3(m_viewMat.xVector.y,
                                               m_viewMat.yVector.y,
                                               m_viewMat.zVector.y) * (m_step * delta))
                                               : position;
    //Update position
    m_viewMat.wVector.x = -position.x;
    m_viewMat.wVector.y = -position.y;
    m_viewMat.wVector.z = -position.z;
  }

  void
  Camera::update(float delta) {
    move(delta);
  }

  void
  Camera::render() {}

  void Camera::setForward(bool state) {
    m_forward = state;
  }

  void Camera::setBackward(bool state) {
    m_back = state;
  }

  void Camera::setLeft(bool state) {
    m_left = state;
  }

  void Camera::setRight(bool state) {
    m_right = state;
  }

  void Camera::setUp(bool state) {
    m_up = state;
  }

  void Camera::setDown(bool state) {
    m_down = state;
  }

  const
  Matrix4 Camera::getView() {
    return m_viewMat;
  }

  const
  Matrix4 Camera::getProjection() {
    return m_projMat;
  }  
}
