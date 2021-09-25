#pragma once
#include <ovPrerequisitesRenderer.h>
#include <ovBaseRenderer.h>
#include <ovVector2.h>

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
  class SamplerState;

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
    SPtr<RasterizerState> m_shadowRS;

    SPtr<DepthStencilState> m_gBufferDS;
    SPtr<DepthStencilState> m_screenQuadDS;

    SPtr<ShaderProgram> m_gBufferProgram;
    SPtr<ShaderProgram> m_ssaoProgram;
    SPtr<ShaderProgram> m_blurHProgram;
    SPtr<ShaderProgram> m_blurVProgram;
    SPtr<ShaderProgram> m_lightProgram;
    SPtr<ShaderProgram> m_shadowProgram;

    Vector<SPtr<Texture>> m_gBufferTextures;
    Vector<SPtr<Texture>> m_ssaoTextures;
    Vector<SPtr<Texture>> m_tempBlurTextures;
    Vector<SPtr<Texture>> m_lightTextures;
    Vector<SPtr<Texture>> m_shadowTextures;

    SPtr<Texture> m_depthStencilTexture;
    SPtr<Texture> m_depthMapTexture;

    SPtr<Buffer> m_gBufferConstant;  
    SPtr<Buffer> m_ssaoBufferConstant;
    SPtr<Buffer> m_blurBufferConstant;
    SPtr<Buffer> m_lightBufferConstant;
    SPtr<Buffer> m_shadowBufferConstant;
    SPtr<Buffer> m_viewInverseBufferConstant;
    
    SPtr<InputLayout> m_gBufferLayout;

    SPtr<InputLayout> m_screenQuadLayout;

    SPtr<SamplerState> m_comparisonSampler;
    SPtr<SamplerState> m_linearSampler;

    Vector2 m_viewportDim;
  };

  extern "C" OV_PLUGIN_EXPORT BaseRenderer*
  createRenderer() {
    auto pRnd = new Renderer();
    return pRnd;
  }
}
