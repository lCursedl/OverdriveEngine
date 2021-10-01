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
    fillKeys();
    fillMouseKeys();
  }
  
  void InputManager::update() {
    m_keyBoard->capture();
    m_mouse->capture();
  }

  bool
  InputManager::isKeyPressed(KEYS::E key) {
    return m_keyBoard->isKeyDown(m_keys[key]);
  }

  bool
  InputManager::isMouseKeyPressed(KEYSM::E key) {
    return m_mouse->getMouseState().buttonDown(m_mouseKeys[key]);
  }

  void
  InputManager::getMouseAxis(float& X, float& Y) {
    const OIS::MouseState& ms = m_mouse->getMouseState();
    X = ms.X.rel;
    Y = ms.Y.rel;
  }

  void
  InputManager::fillKeys() {
    m_keys.insert(std::make_pair(KEYS::kESCAPE, OIS::KeyCode::KC_ESCAPE));
    m_keys.insert(std::make_pair(KEYS::k1, OIS::KeyCode::KC_1));
    m_keys.insert(std::make_pair(KEYS::k2, OIS::KeyCode::KC_2));
    m_keys.insert(std::make_pair(KEYS::k3, OIS::KeyCode::KC_3));
    m_keys.insert(std::make_pair(KEYS::k4, OIS::KeyCode::KC_4));
    m_keys.insert(std::make_pair(KEYS::k5, OIS::KeyCode::KC_5));
    m_keys.insert(std::make_pair(KEYS::k6, OIS::KeyCode::KC_6));
    m_keys.insert(std::make_pair(KEYS::k7, OIS::KeyCode::KC_7));
    m_keys.insert(std::make_pair(KEYS::k8, OIS::KeyCode::KC_8));
    m_keys.insert(std::make_pair(KEYS::k9, OIS::KeyCode::KC_9));
    m_keys.insert(std::make_pair(KEYS::k0, OIS::KeyCode::KC_0));
    m_keys.insert(std::make_pair(KEYS::kMINUS, OIS::KeyCode::KC_MINUS));
    m_keys.insert(std::make_pair(KEYS::kEQUALS, OIS::KeyCode::KC_EQUALS));
    m_keys.insert(std::make_pair(KEYS::kBACK, OIS::KeyCode::KC_BACK));
    m_keys.insert(std::make_pair(KEYS::kTAB, OIS::KeyCode::KC_TAB));
    m_keys.insert(std::make_pair(KEYS::kQ, OIS::KeyCode::KC_Q));
    m_keys.insert(std::make_pair(KEYS::kW, OIS::KeyCode::KC_W));
    m_keys.insert(std::make_pair(KEYS::kE, OIS::KeyCode::KC_E));
    m_keys.insert(std::make_pair(KEYS::kR, OIS::KeyCode::KC_R));
    m_keys.insert(std::make_pair(KEYS::kT, OIS::KeyCode::KC_T));
    m_keys.insert(std::make_pair(KEYS::kY, OIS::KeyCode::KC_Y));
    m_keys.insert(std::make_pair(KEYS::kU, OIS::KeyCode::KC_U));
    m_keys.insert(std::make_pair(KEYS::kI, OIS::KeyCode::KC_I));
    m_keys.insert(std::make_pair(KEYS::kO, OIS::KeyCode::KC_O));
    m_keys.insert(std::make_pair(KEYS::kP, OIS::KeyCode::KC_P));
    m_keys.insert(std::make_pair(KEYS::kA, OIS::KeyCode::KC_A));
    m_keys.insert(std::make_pair(KEYS::kS, OIS::KeyCode::KC_S));
    m_keys.insert(std::make_pair(KEYS::kD, OIS::KeyCode::KC_D));
    m_keys.insert(std::make_pair(KEYS::kF, OIS::KeyCode::KC_F));
    m_keys.insert(std::make_pair(KEYS::kG, OIS::KeyCode::KC_G));
    m_keys.insert(std::make_pair(KEYS::kH, OIS::KeyCode::KC_H));
    m_keys.insert(std::make_pair(KEYS::kJ, OIS::KeyCode::KC_J));
    m_keys.insert(std::make_pair(KEYS::kK, OIS::KeyCode::KC_K));
    m_keys.insert(std::make_pair(KEYS::kL, OIS::KeyCode::KC_L));
    m_keys.insert(std::make_pair(KEYS::kZ, OIS::KeyCode::KC_Z));
    m_keys.insert(std::make_pair(KEYS::kX, OIS::KeyCode::KC_X));
    m_keys.insert(std::make_pair(KEYS::kC, OIS::KeyCode::KC_C));
    m_keys.insert(std::make_pair(KEYS::kV, OIS::KeyCode::KC_V));
    m_keys.insert(std::make_pair(KEYS::kB, OIS::KeyCode::KC_B));
    m_keys.insert(std::make_pair(KEYS::kN, OIS::KeyCode::KC_N));
    m_keys.insert(std::make_pair(KEYS::kM, OIS::KeyCode::KC_M));
    m_keys.insert(std::make_pair(KEYS::kUP, OIS::KeyCode::KC_UP));
    m_keys.insert(std::make_pair(KEYS::kDOWN, OIS::KeyCode::KC_DOWN));
    m_keys.insert(std::make_pair(KEYS::kLEFT, OIS::KeyCode::KC_LEFT));
    m_keys.insert(std::make_pair(KEYS::kRIGHT, OIS::KeyCode::KC_RIGHT));
    m_keys.insert(std::make_pair(KEYS::kLBRACKET, OIS::KeyCode::KC_LBRACKET));
    m_keys.insert(std::make_pair(KEYS::kRBRACKET, OIS::KeyCode::KC_RBRACKET));
    m_keys.insert(std::make_pair(KEYS::kRETURN, OIS::KeyCode::KC_RETURN));
    m_keys.insert(std::make_pair(KEYS::kLCONTROL, OIS::KeyCode::KC_LCONTROL));
    m_keys.insert(std::make_pair(KEYS::kRCONTROL, OIS::KeyCode::KC_RCONTROL));
    m_keys.insert(std::make_pair(KEYS::kSEMICOLON, OIS::KeyCode::KC_SEMICOLON));
    m_keys.insert(std::make_pair(KEYS::kAPOSTROPHE, OIS::KeyCode::KC_APOSTROPHE));
    m_keys.insert(std::make_pair(KEYS::kGRAVE, OIS::KeyCode::KC_GRAVE));
    m_keys.insert(std::make_pair(KEYS::kLSHIFT, OIS::KeyCode::KC_LSHIFT));
    m_keys.insert(std::make_pair(KEYS::kBACKSLASH, OIS::KeyCode::KC_BACKSLASH));
    m_keys.insert(std::make_pair(KEYS::kCOMMA, OIS::KeyCode::KC_COMMA));
    m_keys.insert(std::make_pair(KEYS::kPERIOD, OIS::KeyCode::KC_PERIOD));
    m_keys.insert(std::make_pair(KEYS::kSLASH, OIS::KeyCode::KC_SLASH));
    m_keys.insert(std::make_pair(KEYS::kRSHIFT, OIS::KeyCode::KC_RSHIFT));
    m_keys.insert(std::make_pair(KEYS::kLALT, OIS::KeyCode::KC_LMENU));
    m_keys.insert(std::make_pair(KEYS::kRALT, OIS::KeyCode::KC_RMENU));
    m_keys.insert(std::make_pair(KEYS::kSPACE, OIS::KeyCode::KC_SPACE));
    m_keys.insert(std::make_pair(KEYS::kCAPITAL, OIS::KeyCode::KC_CAPITAL));
    m_keys.insert(std::make_pair(KEYS::kF1, OIS::KeyCode::KC_F1));
    m_keys.insert(std::make_pair(KEYS::kF2, OIS::KeyCode::KC_F2));
    m_keys.insert(std::make_pair(KEYS::kF3, OIS::KeyCode::KC_F3));
    m_keys.insert(std::make_pair(KEYS::kF4, OIS::KeyCode::KC_F4));
    m_keys.insert(std::make_pair(KEYS::kF5, OIS::KeyCode::KC_F5));
    m_keys.insert(std::make_pair(KEYS::kF6, OIS::KeyCode::KC_F6));
    m_keys.insert(std::make_pair(KEYS::kF7, OIS::KeyCode::KC_F7));
    m_keys.insert(std::make_pair(KEYS::kF8, OIS::KeyCode::KC_F8));
    m_keys.insert(std::make_pair(KEYS::kF9, OIS::KeyCode::KC_F9));
    m_keys.insert(std::make_pair(KEYS::kF10, OIS::KeyCode::KC_F10));
    m_keys.insert(std::make_pair(KEYS::kF11, OIS::KeyCode::KC_F11));
    m_keys.insert(std::make_pair(KEYS::kF12, OIS::KeyCode::KC_F12));
    m_keys.insert(std::make_pair(KEYS::kNUMLOCK, OIS::KeyCode::KC_NUMLOCK));
    m_keys.insert(std::make_pair(KEYS::kSCROLL, OIS::KeyCode::KC_SCROLL));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD1, OIS::KeyCode::KC_NUMPAD1));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD2, OIS::KeyCode::KC_NUMPAD2));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD3, OIS::KeyCode::KC_NUMPAD3));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD4, OIS::KeyCode::KC_NUMPAD4));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD5, OIS::KeyCode::KC_NUMPAD5));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD6, OIS::KeyCode::KC_NUMPAD6));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD7, OIS::KeyCode::KC_NUMPAD7));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD8, OIS::KeyCode::KC_NUMPAD8));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD9, OIS::KeyCode::KC_NUMPAD9));
    m_keys.insert(std::make_pair(KEYS::kNUMPAD0, OIS::KeyCode::KC_NUMPAD0));
    m_keys.insert(std::make_pair(KEYS::kSUBTRACT, OIS::KeyCode::KC_SUBTRACT));
    m_keys.insert(std::make_pair(KEYS::kADD, OIS::KeyCode::KC_ADD));
    m_keys.insert(std::make_pair(KEYS::kDECIMAL, OIS::KeyCode::KC_DECIMAL));
    m_keys.insert(std::make_pair(KEYS::kDIVIDE, OIS::KeyCode::KC_DIVIDE));
    m_keys.insert(std::make_pair(KEYS::kMULTIPLY, OIS::KeyCode::KC_MULTIPLY));
    m_keys.insert(std::make_pair(KEYS::kPGDOWN, OIS::KeyCode::KC_PGDOWN));
    m_keys.insert(std::make_pair(KEYS::kPGUP, OIS::KeyCode::KC_PGUP));
    m_keys.insert(std::make_pair(KEYS::kINSERT, OIS::KeyCode::KC_INSERT));
    m_keys.insert(std::make_pair(KEYS::kDELETE, OIS::KeyCode::KC_DELETE));
    m_keys.insert(std::make_pair(KEYS::kHOME, OIS::KeyCode::KC_HOME));
    m_keys.insert(std::make_pair(KEYS::kINSERT, OIS::KeyCode::KC_INSERT));
  }

  void
  InputManager::fillMouseKeys() {
    m_mouseKeys.insert(std::make_pair(KEYSM::kLEFT, OIS::MouseButtonID::MB_Left));
    m_mouseKeys.insert(std::make_pair(KEYSM::kRIGHT, OIS::MouseButtonID::MB_Right));
    m_mouseKeys.insert(std::make_pair(KEYSM::kMIDDLE, OIS::MouseButtonID::MB_Middle));
    m_mouseKeys.insert(std::make_pair(KEYSM::kBUTTON3, OIS::MouseButtonID::MB_Button3));
    m_mouseKeys.insert(std::make_pair(KEYSM::kBUTTON4, OIS::MouseButtonID::MB_Button4));
    m_mouseKeys.insert(std::make_pair(KEYSM::kBUTTON5, OIS::MouseButtonID::MB_Button5));
    m_mouseKeys.insert(std::make_pair(KEYSM::kBUTTON6, OIS::MouseButtonID::MB_Button6));
    m_mouseKeys.insert(std::make_pair(KEYSM::kBUTTON7, OIS::MouseButtonID::MB_Button7));
  }
  
  bool
  EventHandler::keyPressed(const OIS::KeyEvent& arg) {

    /*std::cout << " KeyPressed {" << std::hex << arg.key << std::dec
      << "} || Character (" << (char)arg.text << ")" << std::endl;*/
  
    return true;
  }
  
  bool EventHandler::keyReleased(const OIS::KeyEvent& arg) {
    //std::cout << "KeyReleased {" << std::hex << arg.key << std::dec << "}\n";
    return true;
  }
  
  bool EventHandler::mouseMoved(const OIS::MouseEvent& arg) {
    /*const OIS::MouseState& s = arg.state;
    std::cout << "\nMouseMoved: Abs("
              << s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel
              << ", " << s.Y.rel << ", " << s.Z.rel << ")";*/
    return true;
  }
  
  bool EventHandler::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    /*const OIS::MouseState& s = arg.state;
    std::cout << "\nMouse button #" << id << " pressed. Abs(" << s.X.abs << ", "
              << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel << ", " << s.Y.rel
              << ", " << s.Z.rel << ")";*/
    return true;
  }
  
  bool EventHandler::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    /*const OIS::MouseState& s = arg.state;
    std::cout << "\nMouse button #" << id << " released. Abs(" << s.X.abs << ", "
              << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel << ", " << s.Y.rel
              << ", " << s.Z.rel << ")";*/
    return true;
  }
}