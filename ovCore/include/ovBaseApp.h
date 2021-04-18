#pragma once
#include <SFML/Window.hpp>
#include <ovPrerequisitesCore.h>
#include <ovGraphicsAPI.h>
#include <ovPlugin.h>

using sf::WindowHandle;
using sf::Event;

namespace ovEngineSDK {
  class OV_PLUGIN_EXPORT BaseApp
  {
   public:
    BaseApp() = default;
    virtual ~BaseApp() = default;
    int32
    run();
    void eventHandler(sf::Event event);
   protected:
    
    /*
    * @fn        create()
    * @brief     Initialilizes all systems in base app.
    */
    virtual void
    onCreate();

    /*
    * @fn        onUpdate()
    * @brief     
    */
    virtual void
    onUpdate();

    /*
    * @fn
    * @brief
    */
    virtual void
    onRender();

    /*
    * @fn
    * @brief
    */
    virtual void
    onClear();

   private:
    void
    createWindow();

    void
    update(float delta);

    void
    render();

    void
    initSystems();

    void
    destroySystems();
   protected:
    WindowHandle m_windowHandle;
    Plugin m_directXPlugin;
    GraphicsAPI* m_graphicsAPI = nullptr;
  };
}