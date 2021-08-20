#pragma once
#include <ovModule.h>
#include <ovPrerequisitesCore.h>

namespace ovEngineSDK {

  class BaseRenderer : public Module<BaseRenderer>
  {
   public:
    BaseRenderer() = default;
    virtual ~BaseRenderer() = default;

    virtual void
    init() {}
    virtual void
    render() {}

    FORCEINLINE void
    setObject(BaseRenderer* _api) {
    BaseRenderer::_instance() = _api;
    }

  };

  OV_CORE_EXPORT BaseRenderer&
  g_baseRenderer();

  using funcCreateBaseRenderer = BaseRenderer * (*)();
}