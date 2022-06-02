#pragma once
#include <ovModule.h>
#include <ovTexture.h>
#include <ovMatrix4.h>
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
    
    virtual void
    setTransformCB(Matrix4 Transform) { OV_UNREFERENCED_PARAMETER(Transform); }

    friend class GameApp;
    friend class Actor;
  };

  OV_CORE_EXPORT BaseRenderer&
  g_baseRenderer();

  using funcCreateBaseRenderer = BaseRenderer * (*)();
}