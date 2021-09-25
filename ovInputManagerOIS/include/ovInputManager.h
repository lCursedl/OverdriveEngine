#pragma once
#include <ovPrerequisitesInputManager.h>
#include <ovBaseInputManager.h>

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>
#include <OISException.h>


namespace ovEngineSDK {
  
  class EventHandler : public OIS::KeyListener, public OIS::MouseListener
  {
   public:
    EventHandler() = default;
    ~EventHandler() = default;
    
    bool keyPressed(const OIS::KeyEvent& arg)                         override;
    bool keyReleased(const OIS::KeyEvent& arg)                        override;
    bool mouseMoved(const OIS::MouseEvent& arg)                       override;
    bool mousePressed(const OIS::MouseEvent& arg,
                      OIS::MouseButtonID id)                          override;
    bool mouseReleased(const OIS::MouseEvent& arg,
                       OIS::MouseButtonID id)                         override;
  };

  class InputManager final : public BaseInputManager
  {
   public:
    InputManager() = default;
    ~InputManager();

    void
    init(WindowHandle wHndl)                                          override;
    void
    update()                                                          override;

    OIS::InputManager* m_inputManager = nullptr;

    //Devices
    OIS::Keyboard* m_keyBoard = nullptr;
    OIS::Mouse* m_mouse = nullptr;
    EventHandler m_handler;
  };

  extern "C" OV_PLUGIN_EXPORT BaseInputManager*
  createInputManager() {
    auto pIMngr = new InputManager();
    return pIMngr;
  }
}
