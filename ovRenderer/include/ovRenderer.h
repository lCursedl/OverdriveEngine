#pragma once
#include <ovPrerequisitesRenderer.h>
#include <ovBaseRenderer.h>

namespace ovEngineSDK {

  class Model;
  class RasterizerState;
  class DepthStencilState;
  class ShaderProgram;
  class VertexShader;
  class PixelShader;
  class Texture;
  class Buffer;
  class InputLayout;

  class Renderer final : public BaseRenderer
  {
   public:
    Renderer() = default;
    ~Renderer() = default;
    void init() override;
    void render() override;
   private:
    
    Vector<WPtr<Model>> m_models;
    SPtr<Model> m_screenQuad;

    SPtr<RasterizerState> m_gBufferRS;
    SPtr<RasterizerState> m_screenQuadRS;

    SPtr<DepthStencilState> m_gBufferDS;
    SPtr<DepthStencilState> m_screenQuadDS;

    SPtr<ShaderProgram> m_gBufferProgram;
    SPtr<ShaderProgram> m_ssaoProgram;

    Vector<SPtr<Texture>> m_gBufferTextures;
    Vector<SPtr<Texture>> m_ssaoTextures;

    SPtr<Texture> m_depthStencilTexture;

    SPtr<Buffer> m_gBufferConstant;  
    SPtr<Buffer> m_ssaoBufferConstant;
    
    SPtr<InputLayout> m_gBufferLayout;

    SPtr<InputLayout> m_screenQuadLayout;
  };

  extern "C" OV_PLUGIN_EXPORT BaseRenderer*
  createRenderer() {
    auto pRnd = new Renderer();
    return pRnd;
  }
}
