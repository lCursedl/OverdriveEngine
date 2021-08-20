#include "ovRenderer.h"
#include <ovGraphicsAPI.h>
#include <ovModel.h>
#include <ovSceneGraph.h>
#include <ovShaderProgram.h>
#include <ovTexture.h>
#include <ovInputLayout.h>
#include <ovBuffer.h>
#include <ovMatrix4.h>

namespace ovEngineSDK {
  
  struct Matrices {
    Matrix4 view;
    Matrix4 projection;
    Vector4 objectPos;
  };

  struct SSAO {
    float intensity;
    float scale;
    float bias;
    float sample;
  };

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

    m_gBufferLayout = graphicAPI.createInputLayout(m_gBufferProgram, lDesc);

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

    Matrices mat;
    mat.objectPos = Vector4(0.f, 0.f, 300.f, 1.f);
    mat.view = graphicAPI.matrix4Policy(LookAtMatrix(Vector3(0.f, 0.f, -10.f),
                                                     Vector3(0.f, 1.f, 0.f),
                                                     Vector3(0.f, 1.f, 0.f)));
    mat.projection = graphicAPI.matrix4Policy(PerspectiveMatrix(70.f,
                                                                800.f,
                                                                600.f,
                                                                0.01f,
                                                                3000.f));

    m_gBufferConstant = graphicAPI.createBuffer(&mat,
                                                sizeof(Matrices),
                                                BUFFER_TYPE::kCONST_BUFFER);
    
    //SSAO
    m_ssaoProgram = graphicAPI.createShaderProgram();
    m_ssaoProgram->setVertexShader(graphicAPI.createVertexShader(
                                   L"resources/shaders/VS_SSAO"));
    m_ssaoProgram->setPixelShader(graphicAPI.createPixelShader(
                                  L"resources/shaders/PS_SSAO"));
    m_ssaoProgram->linkProgram();

    m_ssaoTextures.push_back(graphicAPI.createTexture(800,
                                                      600,
                                                      TEXTURE_BINDINGS::E::RENDER_TARGET |
                                                        TEXTURE_BINDINGS::E::SHADER_RESOURCE,
                                                      FORMATS::kRGBA16_FLOAT));
    SSAO ssaobuffer;
    ssaobuffer.intensity = 2.f;
    ssaobuffer.scale = .8f;
    ssaobuffer.bias = .03f;
    ssaobuffer.sample = 1.f;

    m_ssaoBufferConstant = graphicAPI.createBuffer(&ssaobuffer,
                                                   sizeof(SSAO),
                                                   BUFFER_TYPE::kCONST_BUFFER);

    //Screen Aligned Quad
    m_screenQuadRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                      CULL_MODE::kNONE,
                                                      false);

    
    m_screenQuadDS = graphicAPI.createDepthStencilState(false, false);

    m_screenQuad = make_shared<Model>();
    m_screenQuad->load("resources/models/ScreenAlignedQuad.3ds");

    m_screenQuadLayout = graphicAPI.createInputLayout(m_ssaoProgram, lDesc);
    

  }

  void
  Renderer::render() {

    Color clearColor;
    clearColor.red = 0.0f;
    clearColor.green = 0.0f;
    clearColor.blue = 0.0f;
    clearColor.alpha = 1.f;

    auto& graphicAPI = g_graphicsAPI();
    
    //GBuffer
    graphicAPI.setInputLayout(m_gBufferLayout);
    graphicAPI.setShaders(m_gBufferProgram);
    graphicAPI.setRasterizerState(m_gBufferRS);
    graphicAPI.setDepthStencilState(m_gBufferDS);

    graphicAPI.setConstantBuffer(0, m_gBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_gBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    for (auto& target : m_gBufferTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.clearDepthStencil(m_depthStencilTexture);

    graphicAPI.setRenderTarget(m_gBufferTextures.size(),
                               m_gBufferTextures,
                               m_depthStencilTexture);
    SceneGraph::instance().render();

    //SSAO
    graphicAPI.setInputLayout(m_screenQuadLayout);
    graphicAPI.setShaders(m_ssaoProgram);
    graphicAPI.setRasterizerState(m_screenQuadRS);
    graphicAPI.setDepthStencilState(m_screenQuadDS);

    graphicAPI.setConstantBuffer(0, m_ssaoBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_ssaoBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    for (auto& target : m_ssaoTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.setRenderTarget(1, m_ssaoTextures, m_depthStencilTexture);

    graphicAPI.setTexture(0, m_gBufferTextures[0]);
    graphicAPI.setTexture(1, m_gBufferTextures[1]);

    m_screenQuad->render();
  }
}
