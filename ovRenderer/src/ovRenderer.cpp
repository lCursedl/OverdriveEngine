#include "ovRenderer.h"
#include <ovGraphicsAPI.h>
#include <ovModel.h>
#include <ovSceneGraph.h>
#include <ovShaderProgram.h>
#include <ovTexture.h>
#include <ovInputLayout.h>
#include <ovBuffer.h>
#include <ovMatrix4.h>
#include <ovCamera.h>

namespace ovEngineSDK {
  
  struct Matrices {
    Matrix4 view;
    Matrix4 projection;
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
    SPtr<Texture> backTexture;
    graphicAPI.getBackBuffer(backTexture);
    m_backBufferTextures.push_back(backTexture);
    m_viewportDim = graphicAPI.getViewportDimensions();
    //GBuffer
    m_gBufferRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                   CULL_MODE::kFRONT,
                                                    true, false);
    
    m_gBufferDS = graphicAPI.createDepthStencilState(true, true, COMPARISON::LESS);

    m_linearSampler = graphicAPI.createSamplerState(FILTER_LEVEL::FILTER_LINEAR,
                                                    FILTER_LEVEL::FILTER_LINEAR,
                                                    FILTER_LEVEL::FILTER_LINEAR,
                                                    false,
                                                    0,
                                                    WRAPPING::WRAP,
                                                    COMPARISON::NEVER);

    m_gBufferProgram = graphicAPI.createShaderProgram();
    m_gBufferProgram->setVertexShader(graphicAPI.createVertexShader(
                                      L"resources/shaders/VertexGBuffer"));
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
        static_cast<int32>(m_viewportDim.x),
        static_cast<int32>(m_viewportDim.y),
        TEXTURE_BINDINGS::E::RENDER_TARGET | TEXTURE_BINDINGS::E::SHADER_RESOURCE,
        FORMATS::kRGBA16_FLOAT));
    }

    m_depthStencilTexture = graphicAPI.createTexture(static_cast<int32>(m_viewportDim.x),
                                                     static_cast<int32>(m_viewportDim.y),
                                                     TEXTURE_BINDINGS::E::DEPTH_STENCIL,
                                                     FORMATS::kD24_S8);

    m_gBufferConstant = graphicAPI.createBuffer(nullptr,
                                                sizeof(Matrices),
                                                BUFFER_TYPE::kCONST_BUFFER);

    m_gBufferModel = graphicAPI.createBuffer(nullptr,
                                             sizeof(Matrix4),
                                             BUFFER_TYPE::kCONST_BUFFER);

    m_viewInverseBufferConstant = graphicAPI.createBuffer(nullptr,
                                                          sizeof(Matrix4),
                                                          BUFFER_TYPE::kCONST_BUFFER);


    //SSAO
    m_ssaoProgram = graphicAPI.createShaderProgram();
    m_ssaoProgram->setComputeShader(graphicAPI.createComputeShader(
                                    L"resources/shaders/CS_SSAO"));
    m_ssaoProgram->linkProgram();

    m_ssaoTextures.push_back(graphicAPI.createTexture(
                             static_cast<int32>(m_viewportDim.x),
                             static_cast<int32>(m_viewportDim.y),
                             TEXTURE_BINDINGS::E::RENDER_TARGET |
                             TEXTURE_BINDINGS::E::SHADER_RESOURCE |
                             TEXTURE_BINDINGS::E::UNORDERED_ACCESS,
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
    m_blurHProgram->setComputeShader(graphicAPI.createComputeShader(
                                     L"resources/shaders/CS_BlurH"));
    m_blurHProgram->linkProgram();
    m_tempBlurTextures.push_back(graphicAPI.createTexture(
                                 static_cast<int32>(m_viewportDim.x),
                                 static_cast<int32>(m_viewportDim.y),
                                 TEXTURE_BINDINGS::E::RENDER_TARGET |
                                 TEXTURE_BINDINGS::E::SHADER_RESOURCE |
                                 TEXTURE_BINDINGS::E::UNORDERED_ACCESS,
                                 FORMATS::kRGBA16_FLOAT));

    //BlurV
    m_blurVProgram = graphicAPI.createShaderProgram();
    m_blurVProgram->setComputeShader(graphicAPI.createComputeShader(
                                     L"resources/shaders/CS_BlurV"));
    m_blurVProgram->linkProgram();

    //Light
    m_lightProgram = graphicAPI.createShaderProgram();
    m_lightProgram->setVertexShader(graphicAPI.createVertexShader(
                                    L"resources/shaders/VS_SSAO"));
    m_lightProgram->setPixelShader(graphicAPI.createPixelShader(
                                   L"resources/shaders/PS_Light"));
    m_lightProgram->linkProgram();

    m_lightBufferConstant = graphicAPI.createBuffer(nullptr,
                                                    sizeof(Lighting),
                                                    BUFFER_TYPE::kCONST_BUFFER);
    m_outputTexture.push_back(graphicAPI.createTexture(
                              static_cast<int32>(m_viewportDim.x),
                              static_cast<int32>(m_viewportDim.y),
                              TEXTURE_BINDINGS::E::RENDER_TARGET |
                              TEXTURE_BINDINGS::E::SHADER_RESOURCE,
                              FORMATS::kRGBA16_FLOAT));
    //Shadow map
    m_shadowTextures.push_back(graphicAPI.createTexture(
                                               1024,
                                               1024,
                                               TEXTURE_BINDINGS::E::SHADER_RESOURCE |
                                               TEXTURE_BINDINGS::E::RENDER_TARGET,
                                               FORMATS::kR16_FLOAT));
    m_shadowRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                  CULL_MODE::kBACK, false, false);
    m_comparisonSampler = graphicAPI.createSamplerState(FILTER_LEVEL::FILTER_POINT,
                                                        FILTER_LEVEL::FILTER_POINT,
                                                        FILTER_LEVEL::FILTER_POINT,
                                                        false,
                                                        0,
                                                        WRAPPING::CLAMP,
                                                        COMPARISON::NEVER);

    m_shadowProgram = graphicAPI.createShaderProgram();
    m_shadowProgram->setVertexShader(graphicAPI.createVertexShader(
                                     L"resources/shaders/VS_Shadow"));
    m_shadowProgram->setPixelShader(graphicAPI.createPixelShader(
                                    L"resources/shaders/PS_Shadow"));
    m_shadowProgram->linkProgram();
    Matrices lightMat;
    lightMat.projection = graphicAPI.matrix4Policy(graphicAPI.createCompatibleOrtho(-200.f,
                                                            200.f,
                                                            -200.f,
                                                            200.f,
                                                            0.01f,
                                                            500.f));
    //650.f, 300.f, -200.f
    lightMat.view = graphicAPI.matrix4Policy(LookAtMatrix(Vector3(0.f, -80.f, 350.f),
                                                          Vector3(0.f, 0.f, 0.f),
                                                          Vector3(0.f, 1.f, 0.f)));

    //lightMat.objectPos = Vector4(0.f, 0.f, 0.f, 1.f);

    m_shadowBufferConstant = graphicAPI.createBuffer(&lightMat,
                                                     sizeof(Matrices),
                                                     BUFFER_TYPE::kCONST_BUFFER);

    //Screen Aligned Quad
    m_screenQuadRS = graphicAPI.createRasterizerState(FILL_MODE::kSOLID,
                                                      CULL_MODE::kNONE,
                                                      true, false);

    
    m_screenQuadDS = graphicAPI.createDepthStencilState(false, false, COMPARISON::LESS);

    m_screenQuad = Model::load("resources/models/ScreenAlignedQuad.3ds");

    m_screenQuadLayout = graphicAPI.createInputLayout(m_lightProgram, lDesc);
  }

  void
  Renderer::update() {
    auto& graphicAPI = g_graphicsAPI();

    m_activeCam = SceneGraph::instance().getActiveCamera();
    Matrices mat;
    //mat.objectPos = Vector4(0.f, 0.f, 0.f, 1.f);
    mat.projection = graphicAPI.matrix4Policy(m_activeCam->getProjection());
    mat.view = graphicAPI.matrix4Policy(m_activeCam->getView());

    graphicAPI.updateBuffer(m_gBufferConstant, &mat);

    Matrix4 tempMat = graphicAPI.matrix4Policy( m_activeCam->getView().inverse());

    graphicAPI.updateBuffer(m_viewInverseBufferConstant, &tempMat);

    Lighting light;
    light.lightPos = Vector3(0.f, -80.f, 350.f);
    light.lightIntensity = 2.0f;
    light.viewPos = Vector4(mat.view.xVector.w,
                            mat.view.yVector.w,
                            mat.view.zVector.w,
                            1.0f);
    light.matWV = Matrix4::IDENTITY;
    light.matWV.zVector = Vector4(0.f, 0.f, 0.f, 1.0f);//mat.objectPos;
    light.matWV = graphicAPI.matrix4Policy(light.matWV) * mat.view;

    graphicAPI.updateBuffer(m_lightBufferConstant, &light);
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
    graphicAPI.setViewport(0,
                           0,
                           static_cast<int32>(m_viewportDim.x),
                           static_cast<int32>(m_viewportDim.y),
                           0.f,
                           1.f);
    graphicAPI.setRenderTarget(static_cast<int32>(m_gBufferTextures.size()),
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
    graphicAPI.setConstantBuffer(1, m_gBufferModel, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_gBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    
    SceneGraph::instance().render();

    for (int32 i = 0; i < 4; ++i) {
      graphicAPI.setTexture(i, nullptr);
    }

    Vector<SPtr<Texture>> emptyTex;
    graphicAPI.setRenderTarget(4, emptyTex, nullptr);

    //SSAO
    graphicAPI.setTextureUnorderedAccess(0, m_ssaoTextures[0]);

    for (auto& target : m_ssaoTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.setShaders(m_ssaoProgram);    

    graphicAPI.setConstantBuffer(0, m_ssaoBufferConstant, SHADER_TYPE::COMPUTE_SHADER);

    graphicAPI.setTextureShaderResource(0,
                                        m_gBufferTextures[0],
                                        SHADER_TYPE::COMPUTE_SHADER);
    graphicAPI.setTextureShaderResource(1,
                                        m_gBufferTextures[1],
                                        SHADER_TYPE::COMPUTE_SHADER);
    graphicAPI.setSamplerState(0,
                               m_gBufferTextures[0],
                               m_linearSampler,
                               SHADER_TYPE::COMPUTE_SHADER);
    graphicAPI.setSamplerState(0,
                               m_gBufferTextures[1],
                               m_linearSampler,
                               SHADER_TYPE::COMPUTE_SHADER);

    graphicAPI.dispatch(Math::ceil(m_viewportDim.x / 32), Math::ceil(m_viewportDim.y / 32), 1);

    for (int32 i = 0; i < 2; ++i) {
      graphicAPI.setTextureShaderResource(i, nullptr, SHADER_TYPE::COMPUTE_SHADER);
    }

    graphicAPI.setTextureUnorderedAccess(0, nullptr);

    //BlurH - SSAO
    graphicAPI.setTextureUnorderedAccess(0, m_tempBlurTextures[0]);

    for (auto& target : m_tempBlurTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.setShaders(m_blurHProgram);    
    
    Dimension dim;
    Vector2 tempVec = graphicAPI.getViewportDimensions();
    dim.viewportDim.x = tempVec.x;
    dim.viewportDim.y = tempVec.y;

    graphicAPI.updateBuffer(m_blurBufferConstant, &dim);

    graphicAPI.setConstantBuffer(0,
                                 m_blurBufferConstant,
                                 SHADER_TYPE::COMPUTE_SHADER);

    graphicAPI.setTextureShaderResource(0,
                                        m_ssaoTextures[0],
                                        SHADER_TYPE::COMPUTE_SHADER);
    graphicAPI.setSamplerState(0,
                               m_ssaoTextures[0],
                               m_linearSampler,
                               SHADER_TYPE::COMPUTE_SHADER);

    graphicAPI.dispatch(Math::ceil(m_viewportDim.x / 32), Math::ceil(m_viewportDim.y / 32), 1);

    graphicAPI.setTextureShaderResource(0, nullptr, SHADER_TYPE::COMPUTE_SHADER);
    graphicAPI.setTextureUnorderedAccess(0, nullptr);

    //BLURV - SSAO
    graphicAPI.setTextureUnorderedAccess(0, m_ssaoTextures[0]);

    for (auto& target : m_ssaoTextures) {
      graphicAPI.clearRenderTarget(target, clearColor);
    }

    graphicAPI.setShaders(m_blurVProgram);

    graphicAPI.setTextureShaderResource(0,
                                        m_tempBlurTextures[0],
                                        SHADER_TYPE::COMPUTE_SHADER);
    graphicAPI.setSamplerState(0,
                               m_tempBlurTextures[0],
                               m_linearSampler,
                               SHADER_TYPE::COMPUTE_SHADER);

    graphicAPI.dispatch(static_cast<uint32>(Math::ceil(m_viewportDim.x / 32)),
                        static_cast<uint32>(Math::ceil(m_viewportDim.y / 32)),
                        1);
    
    graphicAPI.setTextureShaderResource(0, nullptr, SHADER_TYPE::COMPUTE_SHADER);
    graphicAPI.setTextureUnorderedAccess(0, nullptr);

    //SHADOW MAP
    graphicAPI.setRenderTarget(1, m_shadowTextures, nullptr);
    graphicAPI.clearRenderTarget(m_shadowTextures[0], clearColor);
    graphicAPI.setShaders(m_shadowProgram);
    graphicAPI.setConstantBuffer(0, m_shadowBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_shadowBufferConstant, SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setViewport(0, 0, 1024, 1024, 0.f, 1.f);

    SceneGraph().instance().render();

    //LIGHTING

    graphicAPI.setViewport(0,
                           0,
                           static_cast<int32>(m_viewportDim.x),
                           static_cast<int32>(m_viewportDim.y),
                           0.f,
                           1.f);
    graphicAPI.setRenderTarget(1, m_outputTexture, nullptr);
    graphicAPI.clearRenderTarget(m_outputTexture[0], clearColor);

    graphicAPI.setRasterizerState(m_screenQuadRS);
    graphicAPI.setDepthStencilState(m_screenQuadDS);
    graphicAPI.setInputLayout(m_screenQuadLayout);
    graphicAPI.setShaders(m_lightProgram);
    graphicAPI.setTexture(0, m_gBufferTextures[0]);
    graphicAPI.setTexture(1, m_gBufferTextures[1]);
    graphicAPI.setTexture(2, m_gBufferTextures[2]);
    graphicAPI.setTexture(3, m_ssaoTextures[0]);
    graphicAPI.setTexture(4, m_shadowTextures[0]);

    graphicAPI.setSamplerState(0,
                               m_gBufferTextures[0],
                               m_linearSampler,
                               SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setSamplerState(0,
                               m_gBufferTextures[1],
                               m_linearSampler,
                               SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setSamplerState(0,
                               m_gBufferTextures[2],
                               m_linearSampler,
                               SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setSamplerState(0,
                               m_ssaoTextures[0],
                               m_linearSampler,
                               SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setSamplerState(1,
                               m_shadowTextures[0],
                               m_comparisonSampler,
                               SHADER_TYPE::PIXEL_SHADER);

    graphicAPI.setConstantBuffer(0, m_lightBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(0, m_lightBufferConstant, SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setConstantBuffer(1, m_shadowBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(1, m_shadowBufferConstant, SHADER_TYPE::PIXEL_SHADER);
    graphicAPI.setConstantBuffer(2, m_viewInverseBufferConstant, SHADER_TYPE::VERTEX_SHADER);
    graphicAPI.setConstantBuffer(2, m_viewInverseBufferConstant, SHADER_TYPE::PIXEL_SHADER);

    m_screenQuad->render();

    for (int32 i = 0; i < 5; ++i) {
      graphicAPI.setTexture(i, nullptr);
    }
    graphicAPI.setRenderTarget(1, m_backBufferTextures, nullptr);
    graphicAPI.clearRenderTarget(m_backBufferTextures[0], clearColor);
  }

  void
  Renderer::resize(int32 width, int32 height) {
    m_viewportDim = {static_cast<float>(width), static_cast<float>(height)};
    auto& graphicAPI = g_graphicsAPI();
    for (int32 i = 0; i < 5; ++i) {
      graphicAPI.setTexture(i, nullptr);
    }
    graphicAPI.setTextureUnorderedAccess(0, nullptr);
    graphicAPI.setTextureShaderResource(0, nullptr, SHADER_TYPE::COMPUTE_SHADER);
    //Release light pass
    m_outputTexture.clear();
    //Release shadow pass
    m_shadowTextures.clear();
    //Release Blur-H
    m_tempBlurTextures.clear();
    //Release SSAO
    m_ssaoTextures.clear();
    //Release GBuffer
    m_gBufferTextures.clear();
    m_depthStencilTexture.reset();
    //Release back buffer reference
    m_backBufferTextures.clear();

    for (int32 i = 0; i < 3; ++i) {
      m_gBufferTextures.push_back(graphicAPI.createTexture(
               width,
               height,
               TEXTURE_BINDINGS::E::RENDER_TARGET | TEXTURE_BINDINGS::E::SHADER_RESOURCE,
               FORMATS::kRGBA16_FLOAT));
    }

    m_depthStencilTexture = graphicAPI.createTexture(width,
                                                     height,
                                                     TEXTURE_BINDINGS::E::DEPTH_STENCIL,
                                                     FORMATS::kD24_S8);

    m_ssaoTextures.push_back(graphicAPI.createTexture(
                             width,
                             height,
                             TEXTURE_BINDINGS::E::RENDER_TARGET |
                             TEXTURE_BINDINGS::E::SHADER_RESOURCE |
                             TEXTURE_BINDINGS::E::UNORDERED_ACCESS,
                             FORMATS::kRGBA16_FLOAT));

    m_tempBlurTextures.push_back(graphicAPI.createTexture(
                                 width,
                                 height,
                                 TEXTURE_BINDINGS::E::RENDER_TARGET |
                                 TEXTURE_BINDINGS::E::SHADER_RESOURCE |
                                 TEXTURE_BINDINGS::E::UNORDERED_ACCESS,
                                 FORMATS::kRGBA16_FLOAT));

    m_outputTexture.push_back(graphicAPI.createTexture(
                              width,
                              height,
                              TEXTURE_BINDINGS::E::RENDER_TARGET |
                              TEXTURE_BINDINGS::E::SHADER_RESOURCE,
                              FORMATS::kRGBA16_FLOAT));

    m_shadowTextures.push_back(graphicAPI.createTexture(
                                          1024,
                                          1024,
                                          TEXTURE_BINDINGS::E::SHADER_RESOURCE |
                                          TEXTURE_BINDINGS::E::RENDER_TARGET,
                                          FORMATS::kR16_FLOAT));
  }

  void
  Renderer::storeBackBuffer() {
    SPtr<Texture> backbuffer;
    g_graphicsAPI().getBackBuffer(backbuffer);
    m_backBufferTextures.push_back(backbuffer);
  }

  SPtr<Texture>
  Renderer::getOutputImage() {
    return m_outputTexture.empty() ? nullptr : m_outputTexture[0];
  }
  void
  Renderer::setTransformCB(Matrix4 Transform) {
    //g_graphicsAPI().setConstantBuffer(1, nullptr, SHADER_TYPE::VERTEX_SHADER);
    Matrix4 TransformTrans = g_graphicsAPI().matrix4Policy(Transform);
    g_graphicsAPI().updateBuffer(m_gBufferModel,
                                 &TransformTrans);
    //g_graphicsAPI().setConstantBuffer(1, m_gBufferModel, SHADER_TYPE::VERTEX_SHADER);
  }

  Vector<SPtr<Texture>>
  Renderer::getDeferredTextures() {
    Vector<SPtr<Texture>> deferredText;
    //GBuffer
    for (auto& tex : m_gBufferTextures) {
      deferredText.push_back(tex);
    }
    //SSAO
    for (auto& tex : m_ssaoTextures) {
      deferredText.push_back(tex);
    }
    //Shadows
    for (auto& tex : m_shadowTextures) {
      deferredText.push_back(tex);
    }
    //Light
    for (auto& tex : m_lightTextures) {
      deferredText.push_back(tex);
    }
    return deferredText;
  }
}
