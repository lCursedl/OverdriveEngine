#pragma once
#include <ovModule.h>
#include <ovPrerequisitesCore.h>
#include <SFML/Window.hpp>

using sf::WindowHandle;

namespace ovEngineSDK {

  namespace KEYS {
    enum E {
      kESCAPE = 0,
      k1, 
      k2,
      k3,
      k4,
      k5,
      k6,
      k7,
      k8,
      k9,
      k0,
      kMINUS,
      kEQUALS,
      kBACK,
      kTAB,
      kQ,
      kW,
      kE,
      kR,
      kT,
      kY,
      kU,
      kI,
      kO,
      kP,
      kA,
      kS,
      kD,
      kF,
      kG,
      kH,
      kJ,
      kK,
      kL,
      kZ,
      kX,
      kC,
      kV,
      kB,
      kN,
      kM,
      kUP,
      kDOWN,
      kLEFT,
      kRIGHT,
      kLBRACKET,
      kRBRACKET,
      kRETURN,
      kLCONTROL,
      kRCONTROL,
      kSEMICOLON,
      kAPOSTROPHE,
      kGRAVE,
      kLSHIFT,
      kBACKSLASH,
      kCOMMA,
      kPERIOD,
      kSLASH,
      kRSHIFT,
      kLALT,
      kRALT,
      kSPACE,
      kCAPITAL,
      kF1,
      kF2,
      kF3,
      kF4,
      kF5,
      kF6,
      kF7,
      kF8,
      kF9,
      kF10,
      kF11,
      kF12,
      kNUMLOCK,
      kSCROLL,
      kNUMPAD1,
      kNUMPAD2,
      kNUMPAD3,
      kNUMPAD4,
      kNUMPAD5,
      kNUMPAD6,
      kNUMPAD7,
      kNUMPAD8,
      kNUMPAD9,
      kNUMPAD0,
      kSUBTRACT,
      kADD,
      kDECIMAL,
      kDIVIDE,
      kMULTIPLY,
      kPGDOWN,
      kPGUP,
      kINSERT,
      kDELETE,
      kHOME
    };
  }

  namespace KEYSM {
    enum E {
      kLEFT = 0,
      kRIGHT,
      kMIDDLE,
      kBUTTON3,
      kBUTTON4,
      kBUTTON5,
      kBUTTON6,
      kBUTTON7
    };
  }

  class BaseInputManager : public Module<BaseInputManager>
  {
   public:
    BaseInputManager() = default;
    virtual ~BaseInputManager() = default;

    virtual void
    init(WindowHandle wHndl) {}

    virtual void
    update() {}

    virtual bool
    isKeyPressed(KEYS::E key) {return false;}

    virtual bool
    isMouseKeyPressed(KEYSM::E key) {return false;}

    virtual float
    getXAxis() { return 0.f; }

    virtual float
    getYAxis() { return 0.f; }

    virtual void
    getMouseAxis(float& X, float& Y) {}

    FORCEINLINE void
    setObject(BaseInputManager* _api) {
      BaseInputManager::_instance() = _api;
    }
  };

  OV_CORE_EXPORT BaseInputManager&
  g_baseInput();

  using funcCreateBaseInputManager = BaseInputManager * (*)();
}