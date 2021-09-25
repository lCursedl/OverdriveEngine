#pragma once
#include <ovModule.h>
#include <ovPrerequisitesCore.h>
#include <SFML/Window.hpp>

using sf::WindowHandle;

namespace ovEngineSDK {  

  class BaseInputManager : public Module<BaseInputManager>
  {
   public:
    BaseInputManager() = default;
    virtual ~BaseInputManager() = default;

    virtual void
    init(WindowHandle wHndl) {}

    virtual void
    update() {}

    FORCEINLINE void
    setObject(BaseInputManager* _api) {
      BaseInputManager::_instance() = _api;
    }
  };

  OV_CORE_EXPORT BaseInputManager&
  g_baseInput();

  using funcCreateBaseInputManager = BaseInputManager * (*)();
}