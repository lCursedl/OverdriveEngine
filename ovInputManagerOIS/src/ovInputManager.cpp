#include "ovInputManager.h"

namespace ovEngineSDK {
  InputManager::~InputManager() {
    if (m_inputManager) {
      OIS::InputManager::destroyInputSystem(m_inputManager);
    }
  }

  void
  InputManager::init(WindowHandle wHndl) {
    OIS::ParamList pl;
    std::ostringstream windowHndStr;
    windowHndStr << (SIZE_T)wHndl;
    pl.insert(std::make_pair(String("WINDOW"), windowHndStr.str()));
  
    m_inputManager = OIS::InputManager::createInputSystem(pl);
    uint32 v = m_inputManager->getVersionNumber();
    std::cout << "OIS Version" << (v >> 16) << "." << ((v >> 8) & 0x000000FF) << "." << (v & 0x000000FF)
              << "\nRelease Name: " << m_inputManager->getVersionName()
              << "\nManager: " << m_inputManager->inputSystemName()
              << "\nTotal Keyboards: " << m_inputManager->getNumberOfDevices(OIS::OISKeyboard)
              << "\nTotal Mice: " << m_inputManager->getNumberOfDevices(OIS::OISMouse)
              << "\nTotal JoySticks: " << m_inputManager->getNumberOfDevices(OIS::OISJoyStick);
  
    m_keyBoard = static_cast<OIS::Keyboard*>(
                  m_inputManager->createInputObject(OIS::OISKeyboard, true));
    m_keyBoard->setEventCallback(&m_handler);
  
    m_mouse = static_cast<OIS::Mouse*>(
              m_inputManager->createInputObject(OIS::OISMouse, true));
    m_mouse->setEventCallback(&m_handler);
    const OIS::MouseState& ms = m_mouse->getMouseState();
    ms.width = 800;
    ms.height = 600;
  }
  
  void InputManager::update() {
    m_keyBoard->capture();
    m_mouse->capture();
  }
  
  bool
  EventHandler::keyPressed(const OIS::KeyEvent& arg) {
    std::cout << " KeyPressed {" << std::hex << arg.key << std::dec
      << "} || Character (" << (char)arg.text << ")" << std::endl;
  
    if (arg.key == OIS::KeyCode::KC_UP) {
      std::cout << "up key!\n";
    }
  
    return true;
  }
  
  bool EventHandler::keyReleased(const OIS::KeyEvent& arg) {
    std::cout << "KeyReleased {" << std::hex << arg.key << std::dec << "}\n";
    return true;
  }
  
  bool EventHandler::mouseMoved(const OIS::MouseEvent& arg) {
    const OIS::MouseState& s = arg.state;
    std::cout << "\nMouseMoved: Abs("
              << s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel
              << ", " << s.Y.rel << ", " << s.Z.rel << ")";
    return true;
  }
  
  bool EventHandler::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    const OIS::MouseState& s = arg.state;
    std::cout << "\nMouse button #" << id << " pressed. Abs(" << s.X.abs << ", "
              << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel << ", " << s.Y.rel
              << ", " << s.Z.rel << ")";
    return true;
  }
  
  bool EventHandler::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    const OIS::MouseState& s = arg.state;
    std::cout << "\nMouse button #" << id << " released. Abs(" << s.X.abs << ", "
              << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel << ", " << s.Y.rel
              << ", " << s.Z.rel << ")";
    return true;
  }
}