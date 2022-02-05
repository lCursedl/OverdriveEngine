#pragma once
#include <ovModule.h>
#include <ovPrerequisitesCore.h>

namespace ovEngineSDK {
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
    loadUSD(const String& fileName) { return false; }

  }; 
  
  OV_CORE_EXPORT BaseOmniverse&
  g_baseOmniverse();

  using funcCreateBaseOmniverse = BaseOmniverse * (*)();
}