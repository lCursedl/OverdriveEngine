#include "ovRenderer.h"
#include <ovGraphicsAPI.h>
#include <ovModel.h>
#include <ovSceneGraph.h>
#include <ovShaderProgram.h>
#include <ovTexture.h>
#include <ovInputLayout.h>

namespace ovEngineSDK {
  void
  Renderer::init() {
    auto& graphicAPI =  g_graphicsAPI();
    //GBuffer
    m_gBufferRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                   CULL_MODE::kFRONT,
                                                    true);
    
    m_gBufferDS = graphicAPI.createDepthStencilState(true, true);
    SPtr<VertexShader> gBufferVertex = graphicAPI.createVertexShader(
                                       L"resources/shaders/VertexGBuffer");
    SPtr<PixelShader> gBufferPixel = graphicAPI.createPixelShader(
                                       L"resources/shaders/PixelGBuffer");
    m_gBufferProgram = graphicAPI.createShaderProgram();
    m_gBufferProgram->setVertexShader(gBufferVertex);
    m_gBufferProgram->setPixelShader(gBufferPixel);
    m_gBufferProgram->linkProgram();

    LAYOUT_DESC lDesc;
    lDesc.addToDesc(SEMANTIC::kPOSITION, FORMATS::kRGB32_FLOAT, 0, 3);
    lDesc.addToDesc(SEMANTIC::kTEXCOORD, FORMATS::kRG32_FLOAT, 12, 2);
    lDesc.addToDesc(SEMANTIC::kNORMAL, FORMATS::kRGB32_FLOAT, 20, 3);
    lDesc.addToDesc(SEMANTIC::kTANGENT, FORMATS::kRGB32_FLOAT, 32, 3);
    lDesc.addToDesc(SEMANTIC::kBINORMAL, FORMATS::kRGB32_FLOAT, 44, 3);

    SPtr<InputLayout>layout = graphicAPI.createInputLayout(m_gBufferProgram, lDesc);
    graphicAPI.setInputLayout(layout);

    for (int32 i = 0; i < 3; ++i) {
      m_gBufferTextures.push_back(graphicAPI.createTexture(
        800,
        600,
        TEXTURE_BINDINGS::E::RENDER_TARGET | TEXTURE_BINDINGS::E::SHADER_RESOURCE,
        FORMATS::kRGBA16_FLOAT));
    }

    m_depthStencilTexture = graphicAPI.createTexture(800,
                                                    600,
                                                    TEXTURE_BINDINGS::E::DEPTH_STENCIL,
                                                    FORMATS::kD24_S8);

  graphicAPI.setShaders(m_gBufferProgram);
    //Screen Aligned Quad
    /*m_screenQuadRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                      CULL_MODE::kNONE,
                                                      false);

    
    m_screenQuadDS = graphicAPI.createDepthStencilState(false, false);

    m_screenQuad = make_shared<Model>();
    m_screenQuad->load("resources/models/ScreenAlignedQuad.3ds");*/

  }

  void
  Renderer::render() {

    Color clearColor;
    clearColor.red = 0.0f;
    clearColor.green = 0.125f;
    clearColor.blue = 0.3f;
    clearColor.alpha = 1.f;

    auto& graphicAPI = g_graphicsAPI();
    
    //graphicAPI.setRasterizerState(m_gBufferRS);
    //graphicAPI.setDepthStencilState(m_gBufferDS);

    for (auto& target : m_gBufferTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.clearDepthStencil(m_depthStencilTexture);

    graphicAPI.setRenderTarget(m_gBufferTextures.size(),
                               m_gBufferTextures,
                               m_depthStencilTexture);
    
    //graphicAPI.clearBackBuffer(clearColor);
    SceneGraph::instance().render();
  }

  void
  Renderer::setModels(Vector<SPtr<Model>> models) {
    for (auto& model : models) {
      m_models.push_back(model);
    }
  }

  OV_RENDERER_EXPORT Renderer& g_renderer() {
    return Renderer::instance();
  }
}
