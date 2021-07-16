#pragma once
#include "ovGameObject.h"

namespace ovEngineSDK {
  
  namespace COMPONENT_TYPE {
    enum E {
      kSTATIC_MESH = 0,
      kSKELETAL_MESH
    };
  };

  class OV_CORE_EXPORT Component : public GameObject
  {
   public:
    Component() = default;
    virtual ~Component() = default;

    virtual void update(float delta);
    virtual void render();

    COMPONENT_TYPE::E m_componentID;
  };
}