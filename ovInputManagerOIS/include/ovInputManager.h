#pragma once
#include <ovPrerequisitesInputManager.h>
#include <ovBaseInputManager.h>

#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OISEvents.h>
#include <OISException.h>


namespace ovEngineSDK {
  class InputManager final : public BaseInputManager,
                             public OIS::KeyListener,
                             public OIS::MouseListener
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
    isKeyReleased(KEYS::E key)                                        override;

    bool
    isMouseKeyPressed(KEYSM::E key)                                   override;

    bool
    isMouseKeyReleased(KEYSM::E key)                                  override;

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

    std::map<OIS::KeyCode, KEYS::E> m_keys;
    std::map<OIS::MouseButtonID, KEYSM::E> m_mouseKeys;

    bool
    keyPressed(const OIS::KeyEvent& arg)                              override;
    bool
    keyReleased(const OIS::KeyEvent& arg)                             override;
    bool
    mouseMoved(const OIS::MouseEvent& arg)                            override;
    bool
    mousePressed(const OIS::MouseEvent& arg,
                      OIS::MouseButtonID id)                          override;
    bool
    mouseReleased(const OIS::MouseEvent& arg,
                        OIS::MouseButtonID id)                        override;

    void
    checkButtonStates();
  };

  extern "C" OV_PLUGIN_EXPORT BaseInputManager*
  createInputManager() {
    auto pIMngr = new InputManager();
    return pIMngr;
  }
}
