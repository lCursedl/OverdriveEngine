#pragma once
#include <SFML/Window.hpp>

using sf::WindowHandle;

namespace ovEngineSDK {
  class BaseApp
  {
   public:
    BaseApp() = default;
    virtual ~BaseApp() = default;

    virtual void create() = 0;
    virtual void update(float delta) = 0;
    virtual void render() = 0;
    virtual void clear() = 0;

   protected:
    WindowHandle m_windowHandle;
  };
}