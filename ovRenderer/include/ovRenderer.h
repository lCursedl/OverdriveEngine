#pragma once
#include <ovModule.h>
#include <ovPrerequisitesRenderer.h>

namespace ovEngineSDK {

  class Model;
  class RasterizerState;
  class DepthStencilState;
  class ShaderProgram;
  class VertexShader;
  class PixelShader;
  class Texture;

  class OV_RENDERER_EXPORT Renderer : public Module<Renderer>
  {
   public:
    Renderer() = default;
    ~Renderer() = default;
    void init();
    void render();
    void setModels(Vector<SPtr<Model>>models);
   private:
    
    Vector<WPtr<Model>> m_models;
    SPtr<Model> m_screenQuad;

    SPtr<RasterizerState> m_gBufferRS;
    SPtr<RasterizerState> m_screenQuadRS;

    SPtr<DepthStencilState> m_gBufferDS;
    SPtr<DepthStencilState> m_screenQuadDS;

    SPtr<ShaderProgram> m_gBufferProgram;

    Vector<SPtr<Texture>> m_gBufferTextures;

    SPtr<Texture> m_depthStencilTexture;
  };

  OV_RENDERER_EXPORT Renderer& g_renderer();
}
