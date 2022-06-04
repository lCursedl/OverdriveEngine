#include "ovActor.h"
#include "ovBaseRenderer.h"

namespace ovEngineSDK {

  Actor::Actor(String name) {
    m_localPosition = Vector3(0.f, 0.f, 0.f);
    m_localRotation = Vector3(0.f, 0.f, 0.f);
    m_localScale = Vector3(1.f, 1.f, 1.f);
    m_localTransform = Matrix4::IDENTITY;
    m_actorName = name;
  }

  void Actor::addComponent(SPtr<Component> component) {
    m_components.push_back(component);
  }

  void Actor::update(float delta) {
    //Scale
    m_localTransform = Matrix4::IDENTITY;
    m_localTransform.xVector.x = m_localScale.x;
    m_localTransform.yVector.y = m_localScale.y;
    m_localTransform.zVector.z = m_localScale.z;
    //Rotation
    Quaternion tempQuat = Quaternion::fromEuler(m_localRotation);
    tempQuat.normalize();
    m_localTransform *= Matrix4::fromQuat(tempQuat);
    //Translation
    m_localTransform.wVector.x = m_localPosition.x;
    m_localTransform.wVector.y = m_localPosition.y;
    m_localTransform.wVector.z = m_localPosition.z;

    SIZE_T numComps = m_components.size();
    for (uint32 i = 0; i < numComps; ++i) {
      m_components[i]->update(delta);
    }
  }

  void Actor::render() {
    g_baseRenderer().setTransformCB(m_localTransform);
    SIZE_T numComps = m_components.size();
    for (SIZE_T i = 0; i < numComps; ++i) {
      m_components[i]->render();
    }
  }

  void
  Actor::setActorName(const String name) {
    m_actorName = name;
  }

  String Actor::getActorName() {
    return m_actorName;
  }
}