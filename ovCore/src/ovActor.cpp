#include "ovActor.h"

namespace ovEngineSDK {
  Actor::Actor() {
    m_localPosition = Vector3(0.f, 0.f, 0.f);
    m_localRotation = Quaternion(0.f, 0.f, 0.f, 0.f);
    m_localScale = Vector3(1.f, 1.f, 1.f);
  }
  void Actor::addComponent(SPtr<Component> component) {
    m_components.push_back(component);
  }
  void Actor::update(float delta) {
    SIZE_T numComps = m_components.size();
    for (uint32 i = 0; i < numComps; ++i) {
      m_components[i]->update(delta);
    }
  }
  void Actor::render() {
    SIZE_T numComps = m_components.size();
    for (SIZE_T i = 0; i < numComps; ++i) {
      m_components[i]->render();
    }
  }
}