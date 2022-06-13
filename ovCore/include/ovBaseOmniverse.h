#pragma once
#include <ovModule.h>
#include <ovVector3.h>
#include <ovPrerequisitesCore.h>

namespace ovEngineSDK {
  
  namespace OMNI_OP {
    enum E {
      kTRANSLATE,
      kROTATE,
      kSCALE
    };
  }

  namespace OMNI_PRECISION {
    enum E {
      kDOUBLE,
      kFLOAT
    };
  }

  class BaseOmniverse : public Module<BaseOmniverse>
  {
   public:
    BaseOmniverse() = default;
    virtual ~BaseOmniverse() = default;

    virtual bool
    init() {return false;}

    virtual void
    update() {}

    FORCEINLINE void
    setObject(BaseOmniverse* omni) {
      BaseOmniverse::_instance() = omni;
    }

    virtual void
    createUSD() {}

    virtual bool
    loadUSD(const String& fileName) {
      OV_UNREFERENCED_PARAMETER(fileName);
      return false;
    }

    virtual bool
    connectFromOmni(const String& fileName) {
      OV_UNREFERENCED_PARAMETER(fileName);
      return false;
    }

    virtual bool
    connectToOmni(const String& fileName) {
      OV_UNREFERENCED_PARAMETER(fileName);
      return false;
    }

    virtual void
    setStagePath(const String& fileName) {
      OV_UNREFERENCED_PARAMETER(fileName);
    }

    virtual void
    setTransformOp(Vector3 data,
                   OMNI_OP::E operation,
                   OMNI_PRECISION::E precision,
                   String omniPath) {
      OV_UNREFERENCED_PARAMETER(data);
      OV_UNREFERENCED_PARAMETER(operation);
      OV_UNREFERENCED_PARAMETER(precision);
      OV_UNREFERENCED_PARAMETER(omniPath);
    }

    virtual bool
    getLiveEdit() { return false; }

  }; 
  
  OV_CORE_EXPORT BaseOmniverse&
  g_baseOmniverse();

  using funcCreateBaseOmniverse = BaseOmniverse * (*)();
}