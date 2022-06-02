#pragma once
#include <ovGameObject.h>
#include <ovComponent.h>
#include <ovMatrix4.h>
#include <ovQuaternion.h>

namespace ovEngineSDK {
  class OV_CORE_EXPORT Actor : public GameObject
  {
   public:
    Actor(String name = "");
    virtual ~Actor() = default;
    void
    addComponent(SPtr<Component> component);
    void
    update(float delta);
    void
    render();

    void
    setActorName(const String name);

    String
    getActorName();

    Vector3 m_localPosition;
    Vector3 m_localRotation;
    Vector3 m_localScale;

    Matrix4 m_localTransform;

    Vector<SPtr<Component>> m_components;

  private:
    String m_actorName;
  };
}