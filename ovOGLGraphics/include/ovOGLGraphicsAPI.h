#pragma once
#include <ovPrerequisitesCore.h>
#include <ovGraphicsAPI.h>

#if OV_PLATFORM == OV_PLATFORM_WIN32
#include <windows.h>
#elif OV_PLATFORM == OV_PLATFORM_LINUX
#endif

namespace ovEngineSDK {
  class OGLGraphicsAPI final : public GraphicsAPI
  {
   public:
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

    SPtr<ShaderProgram>
    createShaderProgram()                                             override;

    SPtr<Buffer>
    createBuffer(const void* data,
                 SIZE_T size,
                 BUFFER_TYPE::E type)                                 override;

    SPtr<InputLayout>
    createInputLayout(SPtr<ShaderProgram> program,
                      LAYOUT_DESC& desc)                               override;
    SPtr<SamplerState>
    createSamplerState(FILTER_LEVEL::E mag,
                       FILTER_LEVEL::E min,
                       FILTER_LEVEL::E mip,
                       uint32 anisotropic,
                       WRAPPING::E wrapMode)                          override;
    SPtr<VertexShader>
    createVertexShader(WString file)                                  override;
    SPtr<PixelShader>
    createPixelShader(WString file)                                   override;

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
    drawIndexed(uint32 indices)                                       override;
    void
    draw(uint32 count, uint32 first)                                  override;
    void
    clearBackBuffer(Color clearColor)                                 override;
    void
    setInputLayout(SPtr<InputLayout> layout)                          override;
    void
    setRenderTarget(SPtr<Texture> texture, SPtr<Texture> depth)       override;
    void
    updateBuffer(SPtr<Buffer> buffer, const void* data)               override;
    void
    setVertexBuffer(SPtr<Buffer> buffer,
                    uint32 stride,
                    uint32 offset)                                    override;
    void
    setIndexBuffer(SPtr<Buffer> buffer)                               override;
    void
    setSamplerState(uint32 slot,
                    SPtr<Texture> texture,
                    SPtr<SamplerState> sampler)                       override;
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

    //SWAPCHAIN

    void
    swapBuffer()                                                      override;
    void
    resizeBackBuffer(uint32 width, uint32 height)                     override;

   private:
    Map<FORMATS::E, std::pair<int32, int32>> m_formats;
    void readShaderFile(std::wstring file, std::string& source);
    HDC m_handle = 0;
    HGLRC oglRenderContext = 0;
    void fillFormats();
    uint32 m_topology = 0;
  };
  extern "C" OV_PLUGIN_EXPORT GraphicsAPI *
    createGraphicsAPI() {
    auto pDX = new OGLGraphicsAPI();
    return pDX;
  }
}