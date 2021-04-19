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
    update(float delta);

    void
    render();

    void
    initSystems();

    void
    destroySystems();

    void
    createWindow();

   protected:
    WindowHandle m_windowHandle = 0;
    Plugin m_directXPlugin;
    GraphicsAPI* m_graphicsAPI = nullptr;
    bool m_runApp = false;

    static LRESULT CALLBACK WndProc(HWND hWnd, uint32 message, WPARAM wParam, LPARAM lParam);
  };
}