#pragma once
#include "ovGameObject.h"

namespace ovEngineSDK {
  class Component : public GameObject
  {
   public:
    Component() = default;
    ~Component() = default;
  };
}