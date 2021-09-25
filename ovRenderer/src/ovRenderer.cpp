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

  struct Dimension {
    Vector4 viewportDim;
  };

  struct Lighting {
    Vector3 lightPos;
    float lightIntensity;
    Vector4 viewPos;
    Matrix4 matWV;
  };

  Matrix4 viewInverse;

  void
  Renderer::init() {
    auto& graphicAPI =  g_graphicsAPI();
    m_viewportDim = graphicAPI.getViewportDimensions();
    //GBuffer
    m_gBufferRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                   CULL_MODE::kFRONT,
                                                    true);
    
    m_gBufferDS = graphicAPI.createDepthStencilState(true, true);

    m_linearSampler = graphicAPI.createSamplerState(FILTER_LEVEL::FILTER_LINEAR,
                                                    FILTER_LEVEL::FILTER_LINEAR,
                                                    FILTER_LEVEL::FILTER_LINEAR,
                                                    0,
                                                    WRAPPING::WRAP,
                                                    COMPARISON::NEVER);

    m_gBufferProgram = graphicAPI.createShaderProgram();
    m_gBufferProgram->setVertexShader(graphicAPI.createVertexShader(L"resources/shaders/VertexGBuffer"));
    m_gBufferProgram->setPixelShader(graphicAPI.createPixelShader(
                                     L"resources/shaders/PixelGBuffer"));
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
  //Vector3(-0.05f, -4.54f, -.8f),
    Matrices mat;
    mat.objectPos = Vector4(0.f, 0.f, 0.f, 1.f);
    mat.view = graphicAPI.matrix4Policy(LookAtMatrix(Vector3(0.f, -150.f, 0.f),
                                                     Vector3(0.f, 0.f, -100.f),
                                                     Vector3(0.f, 1.f, 0.f)));
    mat.projection = graphicAPI.matrix4Policy(PerspectiveMatrix(70.f,
                                                                800.f,
                                                                600.f,
                                                                0.01f,
                                                                3000.f));

    m_gBufferConstant = graphicAPI.createBuffer(&mat,
                                                sizeof(Matrices),
                                                BUFFER_TYPE::kCONST_BUFFER);
    
    viewInverse = graphicAPI.matrix4Policy(mat.view.inverse());

    m_viewInverseBufferConstant = graphicAPI.createBuffer(&viewInverse,
                                                          sizeof(Matrix4),
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

    //BlurH
    m_blurBufferConstant = graphicAPI.createBuffer(nullptr,
                                                   sizeof(Dimension),
                                                   BUFFER_TYPE::kCONST_BUFFER);
    m_blurHProgram = graphicAPI.createShaderProgram();
    m_blurHProgram->setVertexShader(graphicAPI.createVertexShader(
                                    L"resources/shaders/VS_SSAO"));
    m_blurHProgram->setPixelShader(graphicAPI.createPixelShader(
                                   L"resources/shaders/PS_BlurH"));
    m_blurHProgram->linkProgram();
    m_tempBlurTextures.push_back(graphicAPI.createTexture(800,
                                                          600,
                                                          TEXTURE_BINDINGS::E::RENDER_TARGET |
                                                           TEXTURE_BINDINGS::E::SHADER_RESOURCE,
                                                          FORMATS::kRGBA16_FLOAT));

    //BlurV
    m_blurVProgram = graphicAPI.createShaderProgram();
    m_blurVProgram->setVertexShader(graphicAPI.createVertexShader(
                                    L"resources/shaders/VS_SSAO"));
    m_blurVProgram->setPixelShader(graphicAPI.createPixelShader(
                                   L"resources/shaders/PS_BlurV"));
    m_blurVProgram->linkProgram();

    //Light
    m_lightProgram = graphicAPI.createShaderProgram();
    m_lightProgram->setVertexShader(graphicAPI.createVertexShader(
                                    L"resources/shaders/VS_SSAO"));
    m_lightProgram->setPixelShader(graphicAPI.createPixelShader(
                                   L"resources/shaders/PS_Light"));
    m_lightProgram->linkProgram();

    Lighting light;
    light.lightPos = Vector3(650.f, 300.f, -200.f);
    light.lightIntensity = 2.0f;
    light.viewPos = Vector4(mat.view.zVector.x,
                            mat.view.zVector.y,
                            mat.view.zVector.z,
                            1.0f);
    light.matWV = Matrix4::IDENTITY;
    light.matWV.zVector = mat.objectPos;
    light.matWV = graphicAPI.matrix4Policy(light.matWV * mat.view);

    m_lightBufferConstant = graphicAPI.createBuffer(&light,
                                                    sizeof(Lighting),
                                                    BUFFER_TYPE::kCONST_BUFFER);
    //Shadow map
    m_depthMapTexture = graphicAPI.createTexture(1024,
                                                 1024,
                                                 TEXTURE_BINDINGS::E::SHADER_RESOURCE |
                                                 TEXTURE_BINDINGS::E::RENDER_TARGET,
                                                 FORMATS::kR8_UNORM);
    m_shadowRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                  CULL_MODE::kBACK, false);
    m_comparisonSampler = graphicAPI.createSamplerState(FILTER_LEVEL::FILTER_POINT,
                                                        FILTER_LEVEL::FILTER_POINT,
                                                        FILTER_LEVEL::FILTER_POINT,
                                                        0,
                                                        WRAPPING::WRAP,
                                                        COMPARISON::NEVER);

    m_shadowProgram = graphicAPI.createShaderProgram();
    m_shadowProgram->setVertexShader(graphicAPI.createVertexShader(
                                     L"resources/shaders/VS_Shadow"));
    m_shadowProgram->setPixelShader(graphicAPI.createPixelShader(
                                    L"resources/shaders/PS_Shadow"));
    m_shadowProgram->linkProgram();
    Matrices lightMat;
    lightMat.projection = graphicAPI.createCompatibleOrtho(-200.f,
                                                            200.f,
                                                            -200.f,
                                                            200.f,
                                                            0.01f,
                                                            500.f);

    lightMat.view = graphicAPI.matrix4Policy(LookAtMatrix(Vector3(650.f, 300.f, -200.f),
                                                          Vector3(-0.87f, -0.40f, .26f),
                                                          Vector3(0.f, 1.f, 0.f)));

    lightMat.objectPos = Vector4(0.f, 0.f, 0.f, 1.f);

    m_shadowBufferConstant = graphicAPI.createBuffer(&lightMat,
                                                     sizeof(Matrices),
                                                     BUFFER_TYPE::kCONST_BUFFER);    

    m_shadowTextures.push_back(m_depthMapTexture);

    //Screen Aligned Quad
    m_screenQuadRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                      CULL_MODE::kNONE,
                                                      true);

    
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
    graphicAPI.setViewport(0.f, 0.f, m_viewportDim.x, m_viewportDim.y, 0.f, 1.f);
    graphicAPI.setRenderTarget(m_gBufferTextures.size(),
                               m_gBufferTextures,
                               m_depthStencilTexture);

    for (auto& target : m_gBufferTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.clearDepthStencil(m_depthStencilTexture);

    graphicAPI.setInputLayout(m_gBufferLayout);
    graphicAPI.setShaders(m_gBufferProgram);
    graphicAPI.setRasterizerState(m_gBufferRS);
    graphicAPI.setDepthStencilState(m_gBufferDS);

    graphicAPI.setConstantBuffer(0, m_gBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_gBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    
    SceneGraph::instance().render();

    for (int32 i = 0; i < 4; ++i) {
      graphicAPI.setTexture(i, nullptr);
    }

    //SSAO

    graphicAPI.setRenderTarget(1, m_ssaoTextures, nullptr);

    for (auto& target : m_ssaoTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.setInputLayout(m_screenQuadLayout);
    graphicAPI.setShaders(m_ssaoProgram);
    graphicAPI.setRasterizerState(m_screenQuadRS);
    graphicAPI.setDepthStencilState(m_screenQuadDS);

    graphicAPI.setConstantBuffer(0, m_ssaoBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_ssaoBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    graphicAPI.setTexture(0, m_gBufferTextures[0]);
    graphicAPI.setTexture(1, m_gBufferTextures[1]);
    graphicAPI.setSamplerState(0, m_gBufferTextures[0], m_linearSampler);
    graphicAPI.setSamplerState(0, m_gBufferTextures[1], m_linearSampler);

    m_screenQuad->render();

    for (int32 i = 0; i < 2; ++i) {
      graphicAPI.setTexture(i, nullptr);
    }

    //BlurH - SSAO
    graphicAPI.setRenderTarget(1, m_tempBlurTextures, nullptr);

    for (auto& target : m_tempBlurTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.setShaders(m_blurHProgram);    

    Dimension dim;
    Vector2 tempVec = graphicAPI.getViewportDimensions();
    dim.viewportDim.x = tempVec.x;
    dim.viewportDim.y = tempVec.y;

    graphicAPI.updateBuffer(m_blurBufferConstant, &dim);

    graphicAPI.setConstantBuffer(0, m_blurBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_blurBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    graphicAPI.setTexture(0, m_ssaoTextures[0]);
    graphicAPI.setSamplerState(0, m_ssaoTextures[0], m_linearSampler);

    m_screenQuad->render();

    graphicAPI.setTexture(0, nullptr);

    //BLURV - SSAO
    graphicAPI.setRenderTarget(1, m_ssaoTextures, nullptr);

    for (auto& target : m_ssaoTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.setShaders(m_blurVProgram);

    graphicAPI.setTexture(0, m_tempBlurTextures[0]);
    graphicAPI.setSamplerState(0, m_tempBlurTextures[0], m_linearSampler);

    m_screenQuad->render();
    
    graphicAPI.setTexture(0, nullptr);

    //SHADOW MAP
    graphicAPI.setRenderTarget(1, m_shadowTextures, nullptr);
    graphicAPI.clearRenderTarget(m_depthMapTexture, clearColor);
    graphicAPI.setShaders(m_shadowProgram);
    graphicAPI.setConstantBuffer(0, m_shadowBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_shadowBufferConstant, SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setViewport(0, 0, 1024, 1024, 0.f, 1.f);

    SceneGraph().instance().render();

    //LIGHTING

    graphicAPI.setViewport(0.f, 0.f, m_viewportDim.x, m_viewportDim.y, 0.f, 1.f);
    graphicAPI.setBackBuffer();
    graphicAPI.clearBackBuffer(clearColor);

    graphicAPI.setRasterizerState(m_gBufferRS);
    graphicAPI.setShaders(m_lightProgram);
    graphicAPI.setTexture(0, m_gBufferTextures[0]);
    graphicAPI.setTexture(1, m_gBufferTextures[1]);
    graphicAPI.setTexture(2, m_gBufferTextures[2]);
    graphicAPI.setTexture(3, m_ssaoTextures[0]);
    graphicAPI.setTexture(4, m_shadowTextures[0]);

    graphicAPI.setSamplerState(0, m_gBufferTextures[0], m_linearSampler);
    graphicAPI.setSamplerState(0, m_gBufferTextures[1], m_linearSampler);
    graphicAPI.setSamplerState(0, m_gBufferTextures[2], m_linearSampler);
    graphicAPI.setSamplerState(0, m_ssaoTextures[0], m_linearSampler);
    graphicAPI.setSamplerState(1, m_shadowTextures[0], m_comparisonSampler);

    graphicAPI.setConstantBuffer(0, m_lightBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_lightBufferConstant, SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setConstantBuffer(1, m_shadowBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(1, m_shadowBufferConstant, SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setConstantBuffer(2, m_viewInverseBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(2, m_viewInverseBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    m_screenQuad->render();

    for (int32 i = 0; i < 4; ++i) {
      graphicAPI.setTexture(i, nullptr);
    }
  }
}
