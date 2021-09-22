#pragma once
#include <ovPrerequisitesInputManager.h>
#include <ovBaseInputManager.h>

namespace ovEngineSDK {
  
  class InputManager final : public BaseInputManager
  {
   public:
    InputManager() = default;
    ~InputManager() = default;
  };

  extern "C" OV_PLUGIN_EXPORT BaseInputManager*
  createInputManager() {
    auto pIMngr = new InputManager();
    return pIMngr;
  }
}
