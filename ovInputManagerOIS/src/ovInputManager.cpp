#include "ovInputManager.h"
#include <ovGraphicsAPI.h>

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
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
  
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
    m_keyBoard->setEventCallback(this); 
  
    m_mouse = static_cast<OIS::Mouse*>(
              m_inputManager->createInputObject(OIS::OISMouse, true));
    m_mouse->setEventCallback(this);
    const OIS::MouseState& ms = m_mouse->getMouseState();
    Vector2 windowSize = g_graphicsAPI().getViewportDimensions();
    ms.width = static_cast<int32>(windowSize.x);
    ms.height = static_cast<int32>(windowSize.y);
    fillKeys();
    fillMouseKeys();
  }
  
  void InputManager::update() {
    checkButtonStates();
    m_keyBoard->capture();
    m_mouse->capture();
  }

  bool
  InputManager::isKeyPressed(KEYS::E key) {
    return m_keyState[key] == STATUS::kPRESSED ? true : false;
  }

  bool
  InputManager::isKeyReleased(KEYS::E key) {
    return m_keyState[key] == STATUS::kRELEASED ? true : false;
  }

  bool
  InputManager::isMouseKeyPressed(KEYSM::E key) {
    return m_mouseState[key] == STATUS::kPRESSED ? true : false;
  }

  bool
  InputManager::isMouseKeyReleased(KEYSM::E key) {
    return m_mouseState[key] == STATUS::kRELEASED ? true : false;
  }

  void
  InputManager::getMouseAxis(float& X, float& Y) {
    const OIS::MouseState& ms = m_mouse->getMouseState();
    X = (float)ms.X.rel;
    Y = (float)ms.Y.rel;
  }

  void
  InputManager::resizeDimensions(int32 width, int32 height) {
    const OIS::MouseState& ms = m_mouse->getMouseState();
    ms.width = width;
    ms.height = height;
  }

  void
  InputManager::fillKeys() {
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_ESCAPE,     KEYS::kESCAPE));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_1,          KEYS::k1));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_2,          KEYS::k2));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_3,          KEYS::k3));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_4,          KEYS::k4));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_5,          KEYS::k5));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_6,          KEYS::k6));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_7,          KEYS::k7));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_8,          KEYS::k8));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_9,          KEYS::k9));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_0,          KEYS::k0));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_MINUS,      KEYS::kMINUS));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_EQUALS,     KEYS::kEQUALS));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_BACK,       KEYS::kBACK));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_TAB,        KEYS::kTAB));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_Q,          KEYS::kQ));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_W,          KEYS::kW));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_E,          KEYS::kE));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_R,          KEYS::kR));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_T,          KEYS::kT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_Y,          KEYS::kY));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_U,          KEYS::kU));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_I,          KEYS::kI));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_O,          KEYS::kO));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_P,          KEYS::kP));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_A,          KEYS::kA));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_S,          KEYS::kS));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_D,          KEYS::kD));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F,          KEYS::kF));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_G,          KEYS::kG));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_H,          KEYS::kH));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_J,          KEYS::kJ));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_K,          KEYS::kK));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_L,          KEYS::kL));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_Z,          KEYS::kZ));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_X,          KEYS::kX));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_C,          KEYS::kC));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_V,          KEYS::kV));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_B,          KEYS::kB));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_N,          KEYS::kN));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_M,          KEYS::kM));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_UP,         KEYS::kUP));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_DOWN,       KEYS::kDOWN));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_LEFT,       KEYS::kLEFT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_RIGHT,      KEYS::kRIGHT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_LBRACKET,   KEYS::kLBRACKET));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_RBRACKET,   KEYS::kRBRACKET));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_RETURN,     KEYS::kRETURN));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_LCONTROL,   KEYS::kLCONTROL));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_RCONTROL,   KEYS::kRCONTROL));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_SEMICOLON,  KEYS::kSEMICOLON));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_APOSTROPHE, KEYS::kAPOSTROPHE));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_GRAVE,      KEYS::kGRAVE));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_LSHIFT,     KEYS::kLSHIFT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_BACKSLASH,  KEYS::kBACKSLASH));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_COMMA,      KEYS::kCOMMA));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_PERIOD,     KEYS::kPERIOD));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_SLASH,      KEYS::kSLASH));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_RSHIFT,     KEYS::kRSHIFT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_LMENU,      KEYS::kLALT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_RMENU,      KEYS::kRALT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_SPACE,      KEYS::kSPACE));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_CAPITAL,    KEYS::kCAPITAL));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F1,         KEYS::kF1));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F2,         KEYS::kF2));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F3,         KEYS::kF3));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F4,         KEYS::kF4));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F5,         KEYS::kF5));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F6,         KEYS::kF6));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F7,         KEYS::kF7));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F8,         KEYS::kF8));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F9,         KEYS::kF9));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F10,        KEYS::kF10));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F11,        KEYS::kF11));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_F12,        KEYS::kF12));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMLOCK,    KEYS::kNUMLOCK));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_SCROLL,     KEYS::kSCROLL));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD1,    KEYS::kNUMPAD1));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD2,    KEYS::kNUMPAD2));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD3,    KEYS::kNUMPAD3));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD4,    KEYS::kNUMPAD4));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD5,    KEYS::kNUMPAD5));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD6,    KEYS::kNUMPAD6));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD7,    KEYS::kNUMPAD7));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD8,    KEYS::kNUMPAD8));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD9,    KEYS::kNUMPAD9));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_NUMPAD0,    KEYS::kNUMPAD0));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_SUBTRACT,   KEYS::kSUBTRACT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_ADD,        KEYS::kADD));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_DECIMAL,    KEYS::kDECIMAL));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_DIVIDE,     KEYS::kDIVIDE));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_MULTIPLY,   KEYS::kMULTIPLY));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_PGDOWN,     KEYS::kPGDOWN));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_PGUP,       KEYS::kPGUP));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_INSERT,     KEYS::kINSERT));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_DELETE,     KEYS::kDELETE));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_HOME,       KEYS::kHOME));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_END,        KEYS::kEND));
    m_keys.insert(std::make_pair(OIS::KeyCode::KC_INSERT,     KEYS::kINSERT));
  }

  void
  InputManager::fillMouseKeys() {
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Left,    KEYSM::kLEFT));
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Right,   KEYSM::kRIGHT));
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Middle,  KEYSM::kMIDDLE));
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Button3, KEYSM::kBUTTON3));
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Button4, KEYSM::kBUTTON4));
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Button5, KEYSM::kBUTTON5));
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Button6, KEYSM::kBUTTON6));
    m_mouseKeys.insert(std::make_pair(OIS::MouseButtonID::MB_Button7, KEYSM::kBUTTON7));
  }

  bool
  InputManager::keyPressed(const OIS::KeyEvent& arg) {
    std::cout << "KeyPressed {" << std::hex << arg.key << std::dec << "}\n";
    m_keyState[m_keys[arg.key]] = STATUS::kPRESSED;
    return true;
  }

  bool
  InputManager::keyReleased(const OIS::KeyEvent& arg) {
    std::cout << "KeyReleased {" << std::hex << arg.key << std::dec << "}\n";
    m_keyState[m_keys[arg.key]] = STATUS::kRELEASED; 
    return true;
  }

  bool
  InputManager::mouseMoved(const OIS::MouseEvent& arg) {
    OV_UNREFERENCED_PARAMETER(arg);
    //const OIS::MouseState& s = arg.state;
    /*std::cout << "\nMouseMoved: Abs("
              << s.X.abs << ", " << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel
              << ", " << s.Y.rel << ", " << s.Z.rel << ")";*/
    return true;
  }

  bool
  InputManager::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    OV_UNREFERENCED_PARAMETER(arg);
    //const OIS::MouseState& s = arg.state;
    /*std::cout << "\nMouse button #" << id << " pressed. Abs(" << s.X.abs << ", "
              << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel << ", " << s.Y.rel
              << ", " << s.Z.rel << ")";*/
    m_mouseState[m_mouseKeys[id]] = STATUS::kPRESSED;
    return false;
  }

  bool
  InputManager::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) {
    OV_UNREFERENCED_PARAMETER(arg);
    //const OIS::MouseState& s = arg.state;
    /*std::cout << "\nMouse button #" << id << " released. Abs(" << s.X.abs << ", "
              << s.Y.abs << ", " << s.Z.abs << ") Rel(" << s.X.rel << ", " << s.Y.rel
              << ", " << s.Z.rel << ")";*/
    m_mouseState[m_mouseKeys[id]] = STATUS::kRELEASED;
    return false;
  }

  void
  InputManager::checkButtonStates() {
    for (auto&  iterator : m_keyState) {
      if (iterator.second == STATUS::kRELEASED) {
        iterator.second = STATUS::kIDLE;
      }
    }

    for (auto& iterator : m_mouseState) {
      if (iterator.second == STATUS::kRELEASED) {
        iterator.second = STATUS::kIDLE;
      }
    }
  }
}