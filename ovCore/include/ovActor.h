#pragma once
#include <ovGameObject.h>
#include <ovComponent.h>
#include <ovMatrix4.h>
#include <ovQuaternion.h>

namespace ovEngineSDK {
  class OV_CORE_EXPORT Actor : public GameObject
  {
   public:
    Actor();
    virtual ~Actor() = default;
    void
    addComponent(SPtr<Component> component);
    void
    update(float delta);
    void
    render();

    Vector3 m_localPosition;
    Quaternion m_localRotation;
    Vector3 m_localScale;

    Vector<SPtr<Component>> m_components; 
  };
}