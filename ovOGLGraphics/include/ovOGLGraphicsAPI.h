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
    bool init(void* window)                                           override;
    void shutdown()                                                   override;
    Matrix4 matrix4Policy(const Matrix4& mat)                         override;

    //DEVICE

    Texture* createTexture(int32 width,
                           int32 height,
                           TEXTURE_BINDINGS::E binding,
                           FORMATS::E format)                         override;

    //Texture* createTextureFromFile(std::string path)                  override;

    ShaderProgram* createShaderProgram()                              override;

    Buffer* createBuffer(const void* data,
                         uint32 size,
                         BUFFER_TYPE::E type)                         override;

    InputLayout* createInputLayout(ShaderProgram* program,
                                   LAYOUT_DESC desc)                  override;
    SamplerState* createSamplerState(FILTER_LEVEL::E mag,
                                     FILTER_LEVEL::E min,
                                     FILTER_LEVEL::E mip,
                                     uint32 anisotropic,
                                     WRAPPING::E wrapMode)            override;
    VertexShader* createVertexShader(std::wstring file)               override;
    PixelShader* createPixelShader(std::wstring file)                 override;

    //DEVICE CONTEXT

    void setBackBuffer()                                              override;
    void setViewport(int32 topLeftX,
                     int32 topLeftY,
                     int32 width,
                     int32 height)                                    override;
    void setShaders(ShaderProgram* program)                           override;
    void drawIndexed(uint32 indices)                                  override;
    void draw(uint32 count, uint32 first)                             override;
    void clearBackBuffer(COLOR color)                                 override;
    void setInputLayout(InputLayout* layout)                          override;
    void setRenderTarget(Texture* texture, Texture* depth)            override;
    void updateBuffer(Buffer* buffer, const void* data)               override;
    void setVertexBuffer(Buffer* buffer,
                         uint32 stride,
                         uint32 offset)                               override;
    void setIndexBuffer(Buffer* buffer)                               override;
    void setSamplerState(uint32 slot,
                         Texture* texture,
                         SamplerState* sampler)                       override;
    void setConstantBuffer(uint32 slot,
                           Buffer* buffer,
                           SHADER_TYPE::E shaderType)                 override;
    void clearRenderTarget(Texture* rt, COLOR color)                  override;
    void clearDepthStencil(Texture* ds)                               override;
    void setTexture(uint32 slot, Texture* texture)                    override;
    void setTopology(TOPOLOGY::E topology)                            override;

    //SWAPCHAIN

    void swapBuffer()                                                 override;
    void resizeBackBuffer(uint32 width, uint32 height)                override;

   private:
    Map<FORMATS::E, std::pair<int32, int32>> m_formats;
    void readShaderFile(std::wstring file, std::string& source);
    HDC m_handle;
    HGLRC oglRenderContext;
    void fillFormats();
    uint32 m_topology;
  };
  extern "C" OV_PLUGIN_EXPORT GraphicsAPI *
    createGraphicsAPI() {
    auto pDX = new OGLGraphicsAPI();
    return pDX;
  }
}