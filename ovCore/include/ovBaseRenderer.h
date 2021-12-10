#pragma once
#include <ovModule.h>
#include <ovTexture.h>
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
    update() {}
    virtual void
    render() {}

    FORCEINLINE void
    setObject(BaseRenderer* _api) {
    BaseRenderer::_instance() = _api;
    }

   protected:
    virtual SPtr<Texture>
    getOutputImage() { return nullptr;}

    virtual SPtr<Texture>
    getOutputHistogram() { return nullptr; }

    friend class GameApp;
  };

  OV_CORE_EXPORT BaseRenderer&
  g_baseRenderer();

  using funcCreateBaseRenderer = BaseRenderer * (*)();
}