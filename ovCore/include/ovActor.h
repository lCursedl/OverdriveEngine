#pragma once
#include <ovGameObject.h>
#include <ovComponent.h>
#include <ovMatrix4.h>

namespace ovEngineSDK {
  class Actor : public GameObject
  {
   public:
    Actor() = default;
    virtual ~Actor() = default;

    Vector3 m_localPosition;
    Vector3 m_localRotation;
    Vector3 m_localScale;

    
  };
}