#pragma once
#include "ovGameObject.h"

namespace ovEngineSDK {
  
  namespace COMPONENT_TYPE {
    enum E {
      kSTATIC_MESH = 0,
      kSKELETAL_MESH,
      kCAMERA
    };
  };

  class OV_CORE_EXPORT Component : public GameObject
  {
   public:
    Component() = default;
    virtual ~Component() = default;

    virtual void update(float delta) = 0;
    virtual void render() = 0;

    COMPONENT_TYPE::E m_componentID;
  };
}