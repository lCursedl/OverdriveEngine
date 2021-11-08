#pragma once
#include <ovPrerequisitesDX.h>

#include <windows.h>
#include <ovGraphicsAPI.h>

namespace ovEngineSDK {
  class DXGraphicsAPI final : public GraphicsAPI
  {
   public:
    DXGraphicsAPI() = default;
    ~DXGraphicsAPI() = default;

    bool
    init(void* window)                                                override;

    void
    shutdown()                                                        override;

    Matrix4
    matrix4Policy(const Matrix4& mat)                                 override;

    //DEVICE

    SPtr<Texture>
    createTexture(int32 width,
                  int32 height,
                  TEXTURE_BINDINGS::E binding,
                  FORMATS::E format)                                  override;

    SPtr<Texture>
    createTextureFromFile(String path)                                override;

    SPtr<Texture>
    createTextureFromMemory(int32 width,
                            int32 height,
                            TEXTURE_BINDINGS::E binding,
                            FORMATS::E format,
                            uint8* data)                              override;

    SPtr<ShaderProgram>
    createShaderProgram()                                             override;

    SPtr<Buffer>
    createBuffer(const void* data,
                 SIZE_T size,
                 BUFFER_TYPE::E type,
                 uint32 elements = 0,
                 FORMATS::E format = FORMATS::kR32_FLOAT)             override;

    SPtr<InputLayout>
    createInputLayout(SPtr<ShaderProgram> program,
                      LAYOUT_DESC& desc)                              override;
    SPtr<SamplerState>
    createSamplerState(FILTER_LEVEL::E mag,
                       FILTER_LEVEL::E min,
                       FILTER_LEVEL::E mip,
                       bool filterCompare,
                       uint32 anisotropic,
                       WRAPPING::E wrapMode,
                       COMPARISON::E compMode)                        override;
    SPtr<VertexShader>
    createVertexShader(WString file)                                  override;
    SPtr<PixelShader>
    createPixelShader(WString file)                                   override;
    SPtr<ComputeShader>
    createComputeShader(WString file)                                 override;

    SPtr<RasterizerState>
    createRasterizerState(FILL_MODE::E fillMode,
                          CULL_MODE::E cullMode,
                          bool counterClockWise,
                          bool scissorEnable)                         override;

    SPtr<DepthStencilState>
    createDepthStencilState(bool stencilEnable,
                            bool depthEnable,
                            COMPARISON::E compMode)                   override;

    Vector2
    getViewportDimensions()                                           override;

    Matrix4
    createCompatibleOrtho(float Left,
                          float Right,
                          float Top,
                          float Bottom,
                          float Near,
                          float Far)                                  override;
    SPtr<BlendState>
    createBlendState(bool enable,
                     BLEND_TYPE::E src,
                     BLEND_TYPE::E dest,
                     BLEND_OP::E operation,
                     BLEND_TYPE::E alphaSrc,
                     BLEND_TYPE::E alphaDest,
                     BLEND_OP::E alphaOp,
                     Vector4 blendFactor)                             override;

    //DEVICE CONTEXT

    void
    setBackBuffer()                                                   override;
    void
    setViewport(int32 topLeftX,
                int32 topLeftY,
                int32 width,
                int32 height,
                float minDepth,
                float maxDepth)                                       override;
    void
    setShaders(SPtr<ShaderProgram> program)                           override;
    void
    drawIndexed(uint32 indices,
                uint32 indexlocation = 0,
                uint32 vertexlocation = 0)                            override;
    void
    draw(uint32 count, uint32 first)                                  override;
    void
    drawIndexedInstanced(uint32 indices,
                        uint32 instances)                             override;
    void
    drawInstanced(uint32 count,
                  uint32 instances,
                  uint32 first)                                       override;
    void
    clearBackBuffer(Color clearColor)                                 override;
    void
    setInputLayout(SPtr<InputLayout> layout)                          override;
    void
    setRenderTarget(int32 amount,
                    Vector<SPtr<Texture>> textures,
                    SPtr<Texture> depth)                              override;
    void
    updateBuffer(SPtr<Buffer> buffer, const void* data)               override;
    void
    setVertexBuffer(SPtr<Buffer> buffer,
                    uint32 stride,
                    uint32 offset)                                    override;
    void
    setIndexBuffer(SPtr<Buffer> buffer, FORMATS::E format)            override;
    void
    setSamplerState(uint32 slot,
                    SPtr<Texture> texture,
                    SPtr<SamplerState> sampler,
                    SHADER_TYPE::E shaderType)                        override;
    void
    setConstantBuffer(uint32 slot,
                      SPtr<Buffer> buffer,
                      SHADER_TYPE::E shaderType)                      override;
    void
    clearRenderTarget(SPtr<Texture> rt, Color clearColor)             override;

    void
    clearDepthStencil(SPtr<Texture> ds)                               override;

    void
    setTexture(uint32 slot, SPtr<Texture> texture)                    override;

    void
    setTopology(TOPOLOGY::E topology)                                 override;

    void
    setRasterizerState(SPtr<RasterizerState> rasterState)             override;

    void
    setDepthStencilState(SPtr<DepthStencilState> depthState)          override;

    void
    dispatch(uint32 threadX, uint32 threadY, uint32 threadZ)          override;

    void
    setBufferShaderResource(uint32 slot,
                      SPtr<Buffer>,
                      SHADER_TYPE::E shader)                          override;

    void
    setTextureShaderResource(uint32 slot,
                      SPtr<Texture> texture,
                      SHADER_TYPE::E shader)                          override;

    void
    setBufferUnorderedAccess(uint32 slot, SPtr<Buffer> buffer)        override;

    void
    setTextureUnorderedAccess(uint32 slot, SPtr<Texture> texture)     override;

    void
    getRaterizerState(SPtr<RasterizerState>& pRS)                     override;

    void
    getBlendState(SPtr<BlendState>& pBS)                              override;

    void
    getDepthStencilState(SPtr<DepthStencilState>& pDSS)               override;

    void
    getTextureShaderResource(uint32 slot,
                             SPtr<Texture>& pTex,
                             SHADER_TYPE::E shaderType)              override;

    void
    getSampler(uint32 slot,
               SPtr<SamplerState>& pSamp,
               SHADER_TYPE::E shaderType)                            override;

    void
    getShaderProgram(SPtr<ShaderProgram>& pProgram)                   override;

    void
    getConstantBuffer(SPtr<Buffer>& pBuffer,
                      uint32 slot,
                      SHADER_TYPE::E shaderType)                     override;

    void
    getBuffer(SPtr<Buffer>& pBuffer, BUFFER_TYPE::E bufferType)      override;

    void
    getVertexBuffer(SPtr<Buffer>& buffer,
                    uint32& stride,
                    uint32& offset)                                   override;

    void
    getIndexBuffer(SPtr<Buffer>& buffer)                              override;

    void
    getInputLayout(SPtr<InputLayout>& pILayout)                       override;

    void
    setScissorRects(float left, float right, float top, float bottom) override;

    //SWAPCHAIN
    void
    swapBuffer()                                                      override;
    void
    resizeBackBuffer(uint32 width, uint32 height)                     override;

    //OUTPUT MERGER
    void
    setBlendState(SPtr<BlendState> blend,
                       uint32 mask)                                   override;
   
    void
    getBackBuffer(SPtr<Texture>& tex)                                 override;

   private:
    ID3D11Device* m_device = nullptr;
    ID3D11DeviceContext* m_deviceContext = nullptr;
    IDXGISwapChain* m_swapChain = nullptr;
    SPtr<Texture> m_backBuffer;
    SPtr<Texture> m_depthStencil;

    std::map<FORMATS::E, DXGI_FORMAT> m_formats;

    HRESULT
    compileShaderFromFile(WString fileName,
                          String shaderModel,
                          ID3DBlob** ppBlobOut);
    void fillFormats();
  };

  extern "C" OV_PLUGIN_EXPORT GraphicsAPI*
  createGraphicsAPI() {
    auto pDX = new DXGraphicsAPI();
    return pDX;
  }
}