#pragma once
#include <ovGameObject.h>
#include <ovComponent.h>
#include <ovMatrix4.h>

namespace ovEngineSDK {
  class OV_CORE_EXPORT Actor : public GameObject/*, public std::enable_shared_from_this<Actor>*/
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

    void
    updateTransform();

    String
    getActorName();

    Vector3 m_localPosition;
    Vector3 m_localRotation;
    Vector3 m_localScale;
    String m_omniPath;
    Matrix4 m_localTransform;

    Vector<SPtr<Component>> m_components;
  private:
    String m_actorName;
  };
}