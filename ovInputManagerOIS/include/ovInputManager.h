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

    bool
    isKeyPressed(KEYS::E key)                                         override;
    bool
    isMouseKeyPressed(KEYSM::E key)                                   override;   

    void
    getMouseAxis(float& X, float& Y)                                  override;

   private:
    
    void
    fillKeys();

    void
    fillMouseKeys();
    
   private:
    OIS::InputManager* m_inputManager = nullptr;
    //Devices
    OIS::Keyboard* m_keyBoard = nullptr;
    OIS::Mouse* m_mouse = nullptr;

    EventHandler m_handler;

    std::map<KEYS::E, OIS::KeyCode> m_keys;
    std::map<KEYSM::E, OIS::MouseButtonID> m_mouseKeys;
  };

  extern "C" OV_PLUGIN_EXPORT BaseInputManager*
  createInputManager() {
    auto pIMngr = new InputManager();
    return pIMngr;
  }
}
