#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <ovOGLGraphicsAPI.h>
#include <ovOGLTexture.h>
#include <ovOGLBuffer.h>
#include <ovOGLShaderProgram.h>
#include <ovOGLInputLayout.h>
#include <ovOGLSamplerState.h>
#include <ovOGLVertexShader.h>
#include <ovOGLPixelShader.h>

namespace ovEngineSDK {
  void
  OGLGraphicsAPI::readShaderFile(std::wstring file, std::string& source) {
    std::ifstream shaderFile;

    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      shaderFile.open(file);
      std::stringstream shaderStream;
      shaderStream << shaderFile.rdbuf();
      shaderFile.close();
      source = shaderStream.str();
    }
    catch (std::ifstream::failure e) {
      return;
    }
  }

  bool
  OGLGraphicsAPI::init(void* window) {
    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,
      0, 0, 0, 0, 0, 0,
      0,
      0,
      0,
      0, 0, 0, 0,
      24,
      8,
      0,
      PFD_MAIN_PLANE,
      0,
      0, 0, 0
    };

    m_handle = GetDC(static_cast<HWND>(window));
    int32 pixelFormat = ChoosePixelFormat(m_handle, &pfd);

    SetPixelFormat(m_handle, pixelFormat, &pfd);

    oglRenderContext = wglCreateContext(m_handle);
    wglMakeCurrent(m_handle, oglRenderContext);

    if (!gladLoadGL()) {
      return false;
    }
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    fillFormats();
    return true;
  }

  void
  OGLGraphicsAPI::shutdown() {
    wglDeleteContext(oglRenderContext);
  }

  Matrix4
  OGLGraphicsAPI::matrix4Policy(const Matrix4& mat) {
      return mat;
  }

  SPtr<Texture>
  OGLGraphicsAPI::createTexture(int32 width,
                                int32 height,
                                TEXTURE_BINDINGS::E binding,
                                FORMATS::E format) {
    SPtr<OGLTexture> Tex(new OGLTexture);
    if (binding & TEXTURE_BINDINGS::E::DEPTH_STENCIL) {
      //Create RenderBufferObject for depth and stencil
      glGenRenderbuffers(1, &Tex->m_texture);
      glBindRenderbuffer(GL_RENDERBUFFER, Tex->m_texture);
      glRenderbufferStorage(GL_RENDERBUFFER, m_formats[format].first, width, height);
      glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
    else {
      //Create texture
      glGenTextures(1, &Tex->m_texture);
      glBindTexture(GL_TEXTURE_2D, Tex->m_texture);
      glTexImage2D(GL_TEXTURE_2D,
        0,
        m_formats[format].first,
        width,
        height,
        0,
        m_formats[format].second,
        GL_UNSIGNED_BYTE,
        NULL);
      if (binding & TEXTURE_BINDINGS::E::RENDER_TARGET) {
        glGenFramebuffers(1, &Tex->m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, Tex->m_framebuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
          GL_COLOR_ATTACHMENT0,
          GL_TEXTURE_2D,
          Tex->m_texture,
          0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }
    }
    return Tex;
  }

  SPtr<Texture>
  OGLGraphicsAPI::createTextureFromFile(String path) {
    int32 width, height, components;
    uint8* data = stbi_load(path.c_str(), &width, &height, &components, 0);
    if (data) {
      GLenum format = GL_ZERO;
      if (components == 1) {
        format = GL_RED;
      }
      else if (components == 2) {
        format = GL_RG;
      }
      else if (components == 3) {
        format = GL_RGB;
      }
      else if (components == 4) {
        format = GL_RGBA;
      }

      SPtr<OGLTexture> texture(new OGLTexture);
      glGenTextures(1, &texture->m_texture);
      glBindTexture(GL_TEXTURE_2D, texture->m_texture);
      glTexImage2D(GL_TEXTURE_2D,
                   0,
                   format,
                   width,
                   height,
                   0,
                   format,
                   GL_UNSIGNED_BYTE,
                   data);
      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glBindTexture(GL_TEXTURE_2D, 0);

      stbi_image_free(data);

      return texture;
    }
    stbi_image_free(data);
    return nullptr;
  }

  WString
  getFileNameOGL(WString vsfile) {
    size_t realPos = 0;
    size_t posInvSlash = vsfile.rfind('\\');
    size_t posSlash = vsfile.rfind('/');

    if (posInvSlash == std::wstring::npos) {
      //No encontramos diagonales invertidas
      if (~posSlash == std::wstring::npos) {
        //Encontramos diagonales normales
        realPos = posSlash;
      }
    }
    else {
      //Encontramos diagonales invertidas
      realPos = posInvSlash;
      if (~posSlash == std::wstring::npos) {
        if (posSlash > realPos) {
          posSlash = realPos;
        }
      }
    }
    return vsfile.substr(realPos, vsfile.length() - realPos);
  }

  SPtr<ShaderProgram>
  OGLGraphicsAPI::createShaderProgram() {
    SPtr<OGLShaderProgram> shaderProgram(new OGLShaderProgram);
    shaderProgram->m_program = glCreateProgram();
    return shaderProgram;
  }

  SPtr<Buffer>
  OGLGraphicsAPI::createBuffer(const void* data,
                               SIZE_T size,
                               BUFFER_TYPE::E type) {
    if (size != 0) {
      SPtr<OGLBuffer> buffer(new OGLBuffer);

      glGenBuffers(1, &buffer->m_buffer);
      buffer->m_size = size;

      switch (type) {
      case BUFFER_TYPE::E::kVERTEX_BUFFER:
        buffer->m_type = GL_ARRAY_BUFFER;
        break;
      case BUFFER_TYPE::E::kINDEX_BUFFER:
        buffer->m_type = GL_ELEMENT_ARRAY_BUFFER;
        break;
      case BUFFER_TYPE::E::kCONST_BUFFER:
        buffer->m_type = GL_UNIFORM_BUFFER;
        break;
      }
      glBindBuffer(buffer->m_type, buffer->m_buffer);
      if (data != nullptr) {
        glBufferData(buffer->m_type, buffer->m_size, data, GL_STATIC_DRAW);
      }
      else {
        glBufferData(buffer->m_type, buffer->m_size, nullptr, GL_STATIC_DRAW);
      }
      glBindBuffer(buffer->m_type, 0);
      return buffer;
    }
    else {
      OutputDebugStringA("Invalid size for buffer\n");
      return nullptr;
    }
  }

  SPtr<InputLayout>
  OGLGraphicsAPI::createInputLayout(SPtr<ShaderProgram> program,
                                    LAYOUT_DESC& desc) {
    SPtr<OGLInputLayout> ILayout(new OGLInputLayout);
    glGenVertexArrays(1, &ILayout->m_vao);
    glBindVertexArray(ILayout->m_vao);
    SIZE_T layoutElements = desc.v_Layout.size();
    for (uint32 i = 0; i < layoutElements; ++i) {
      GLenum varType = desc.v_Layout[i].m_semantic == SEMANTIC::kBLENDINDICES ?
                                                                GL_INT : GL_FLOAT;
      if (varType == GL_INT) {
        glVertexAttribIFormat(i,
          desc.v_Layout[i].m_numElements,
          varType,
          desc.v_Layout[i].m_offset);
      }
      else {
        glVertexAttribFormat(i,
          desc.v_Layout[i].m_numElements,
          varType, GL_FALSE,
          desc.v_Layout[i].m_offset);
      }
      glVertexAttribBinding(i, 0);
      glEnableVertexAttribArray(i);
    }
    glBindVertexArray(0);
    return ILayout;
  }

  SPtr<SamplerState>
  OGLGraphicsAPI::createSamplerState(FILTER_LEVEL::E mag,
                                     FILTER_LEVEL::E min,
                                     FILTER_LEVEL::E mip,
                                     uint32 anisotropic,
                                     WRAPPING::E wrapMode) {
    SPtr<OGLSamplerState> sampler(new OGLSamplerState);

    glGenSamplers(1, &sampler->m_sampler);

    int32 mode = 0;

    switch (wrapMode) {
    case WRAPPING::WRAP:
      mode = GL_REPEAT;
      break;
    case WRAPPING::E::MIRROR:
      mode = GL_MIRRORED_REPEAT;
      break;
    case WRAPPING::E::CLAMP:
      mode = GL_CLAMP_TO_EDGE;
      break;
    case WRAPPING::E::BORDER:
      mode = GL_CLAMP_TO_BORDER;
      break;
    case WRAPPING::E::MIRROR_ONCE:
      mode = GL_MIRROR_CLAMP_TO_EDGE;
      break;
    }

    glSamplerParameteri(sampler->m_sampler,
      GL_TEXTURE_WRAP_S, mode);
    glSamplerParameteri(sampler->m_sampler,
      GL_TEXTURE_WRAP_T, mode);

    if (anisotropic > 0) {
      if (anisotropic > GL_MAX_TEXTURE_MAX_ANISOTROPY) {
        anisotropic = GL_MAX_TEXTURE_MAX_ANISOTROPY;
      }
      glSamplerParameterf(sampler->m_sampler,
                          GL_TEXTURE_MAX_ANISOTROPY,
                          static_cast<float>(anisotropic));

    }

    int32 maglevel = 0;
    switch (mag) {
    case FILTER_LEVEL::E::FILTER_POINT:
      maglevel = GL_NEAREST;
      break;
    case FILTER_LEVEL::E::FILTER_LINEAR:
      maglevel = GL_LINEAR;
      break;
    }

    glSamplerParameteri(sampler->m_sampler, GL_TEXTURE_MAG_FILTER, maglevel);

    int32 minmipLevel = 0;
    if (min == FILTER_LEVEL::E::FILTER_POINT) {
      if (mip == FILTER_LEVEL::E::FILTER_POINT) {
        minmipLevel = GL_NEAREST_MIPMAP_NEAREST;
      }
      else {
        minmipLevel = GL_NEAREST_MIPMAP_LINEAR;
      }
    }
    else if (min == FILTER_LEVEL::E::FILTER_LINEAR) {
      if (mip == FILTER_LEVEL::E::FILTER_LINEAR) {
        minmipLevel = GL_LINEAR_MIPMAP_NEAREST;
      }
      else {
        minmipLevel = GL_LINEAR_MIPMAP_LINEAR;
      }
    }

    glSamplerParameteri(sampler->m_sampler, GL_TEXTURE_MIN_FILTER, minmipLevel);

    return sampler;
  }

  SPtr<VertexShader>
  OGLGraphicsAPI::createVertexShader(WString file) {
    WString realFileName = getFileNameOGL(file) + L"_OGL.glsl";
    String source;
    int32 result;
    char log[512];
    readShaderFile(realFileName, source);
    const char* vs_Source = source.c_str();

    SPtr<OGLVertexShader> vs(new OGLVertexShader);
    vs->m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs->m_vertexShader, 1, &vs_Source, 0);
    glCompileShader(vs->m_vertexShader);

    glGetShaderiv(vs->m_vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
      glGetShaderInfoLog(vs->m_vertexShader, 512, 0, log);
      OutputDebugStringA(log);
      return nullptr;
    }
    return vs;
  }

  SPtr<PixelShader>
  OGLGraphicsAPI::createPixelShader(WString file) {
    WString realFileName = getFileNameOGL(file) + L"_OGL.glsl";
    String source;
    int32 result;
    char log[512];
    readShaderFile(realFileName, source);
    const char* ps_Source = source.c_str();

    SPtr<OGLPixelShader> ps(new OGLPixelShader);
    ps->m_ps = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(ps->m_ps, 1, &ps_Source, 0);
    glCompileShader(ps->m_ps);

    glGetShaderiv(ps->m_ps, GL_COMPILE_STATUS, &result);
    if (!result) {
      glGetShaderInfoLog(ps->m_ps, 512, 0, log);
      OutputDebugStringA(log);
      return nullptr;
    }
    return ps;
  }

  void
  OGLGraphicsAPI::setBackBuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void
  OGLGraphicsAPI::setViewport(int32 topLeftX,
                              int32 topLeftY,
                              int32 width,
                              int32 height,
                              float minDepth,
                              float maxDepth) {
    glViewport(topLeftX, topLeftY, width, height);
    glDepthRange(minDepth, maxDepth);
  }

  void
  OGLGraphicsAPI::setShaders(SPtr<ShaderProgram> program) {
    auto ShaderProgram = static_pointer_cast<OGLShaderProgram>(program);
    glUseProgram(ShaderProgram->m_program);
  }

  void
  OGLGraphicsAPI::drawIndexed(uint32 indices) {
    glDrawElements(m_topology, indices, GL_UNSIGNED_INT, 0);
  }

  void
  OGLGraphicsAPI::draw(uint32 count, uint32 first) {
    glDrawArrays(m_topology, first, count);
  }

  void
  OGLGraphicsAPI::clearBackBuffer(Color clearColor) {
    glClearColor(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void
  OGLGraphicsAPI::setInputLayout(SPtr<InputLayout> layout) {
    glBindVertexArray(static_pointer_cast<OGLInputLayout>(layout)->m_vao);
  }

  void
  OGLGraphicsAPI::setRenderTarget(SPtr<Texture> texture, SPtr<Texture> depth) {
    if (texture) {
      auto tex = static_pointer_cast<OGLTexture>(texture);
      if (tex->m_framebuffer != 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, tex->m_framebuffer);
        if (depth) {
          auto d = static_pointer_cast<OGLTexture>(depth);
          if (d->m_texture != 0) {
            d->m_framebuffer = tex->m_framebuffer;
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,
              GL_DEPTH_STENCIL_ATTACHMENT,
              GL_RENDERBUFFER,
              d->m_texture);
          }
          else {
            OutputDebugStringA("Invalid Depth Stencil received.");
          }
        }
        else {
          glFramebufferRenderbuffer(GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_RENDERBUFFER,
            0);
        }
      }
      else {
        OutputDebugStringA("Invalid RenderTargetView received.");
      }
    }
    else {
      OutputDebugStringA("Received null pointer for texture.");
    }
  }

  void
  OGLGraphicsAPI::updateBuffer(SPtr<Buffer> buffer, const void* data) {
    if (!buffer) {
      OutputDebugStringA("Invalid buffer received.\n");
      return;
    }
    if (!data) {
      OutputDebugStringA("Invalid data received.\n");
      return;
    }

    auto buff = static_pointer_cast<OGLBuffer>(buffer);
    if (buff->m_buffer == 0) {
      OutputDebugStringA("Buffer not initalized, can't update data.\n");
      return;
    }
    glBindBuffer(buff->m_type, buff->m_buffer);
    glBufferSubData(buff->m_type, 0, buff->m_size, data);
    glBindBuffer(buff->m_type, 0);
  }

  void
  OGLGraphicsAPI::setVertexBuffer(SPtr<Buffer> buffer, uint32 stride, uint32 offset) {
    if (buffer != nullptr) {
      auto buff = static_pointer_cast<OGLBuffer>(buffer);
      if (buff->m_buffer != 0) {
        glBindVertexBuffer(0, buff->m_buffer, offset, stride);
      }
      else {
        OutputDebugStringA("Buffer missing initialization.");
      }
    }
    else {
      OutputDebugStringA("Empty buffer received.");
    }
  }

  void
  OGLGraphicsAPI::setIndexBuffer(SPtr<Buffer> buffer) {
    if (buffer != nullptr) {
      auto buff = static_pointer_cast<OGLBuffer>(buffer);
      if (buff->m_buffer != 0) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buff->m_buffer);
      }
      else {
        OutputDebugStringA("Buffer missing initialization.");
      }
    }
    else {
      OutputDebugStringA("Empty buffer received.");
    }
  }

  void
  OGLGraphicsAPI::setSamplerState(uint32 slot,
                                  SPtr<Texture> texture,
                                  SPtr<SamplerState> sampler) {
    if (!texture) {
      OutputDebugStringA("Invalid texture received.");
      return;
    }
    if (!sampler) {
      OutputDebugStringA("Invaid sampler received.");
      return;
    }

    auto tex = static_pointer_cast<OGLTexture>(texture);

    if (tex->m_texture == 0) {
      OutputDebugStringA("Texture missing initialization.");
      return;
    }

    auto samp = static_pointer_cast<OGLSamplerState>(sampler);

    if (samp->m_sampler == 0) {
      OutputDebugStringA("Sampler missing initialization.");
      return;
    }

    glBindSampler(tex->m_texture, samp->m_sampler);
  }

  void
  OGLGraphicsAPI::setConstantBuffer(uint32 slot,
                                    SPtr<Buffer> buffer,
                                    SHADER_TYPE::E shaderType) {
    if (buffer) {
      auto buff = static_pointer_cast<OGLBuffer>(buffer);
      if (buff->m_buffer != 0) {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, buff->m_buffer);
      }
      else {
        OutputDebugStringA("Buffer not initialized received.");
      }
    }
    else {
      OutputDebugStringA("Invalid buffer received.");
    }
  }

  void
  OGLGraphicsAPI::clearRenderTarget(SPtr<Texture> rt, Color clearColor) {
    if (!rt) {
      OutputDebugStringA("Invalid Render Target received.");
      return;
    }
    auto tex = static_pointer_cast<OGLTexture>(rt);
    if (tex->m_framebuffer == 0) {
      OutputDebugStringA("Render target not initialized received.");
      return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, tex->m_framebuffer);
    glClearColor(clearColor.red, clearColor.green, clearColor.blue, clearColor.alpha);
    glClear(GL_COLOR_BUFFER_BIT);
  }

  void
  OGLGraphicsAPI::clearDepthStencil(SPtr<Texture> ds) {
    if (!ds) {
      OutputDebugStringA("Invalid Depth Stencil received.");
      return;
    }
    auto depth = static_pointer_cast<OGLTexture>(ds);
    if (depth->m_texture == 0 || depth->m_framebuffer == 0) {
      OutputDebugStringA("Depth Stencil not initialized received.");
      return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, depth->m_framebuffer);
    glClear(GL_DEPTH_BUFFER_BIT);
  }

  void
  OGLGraphicsAPI::setTexture(uint32 slot, SPtr<Texture> texture) {
    if (!texture) {
      OutputDebugStringA("Texture received was nullptr.");
      return;
    }
    auto tex = static_pointer_cast<OGLTexture>(texture);
    if (tex->m_texture == 0) {
      OutputDebugStringA("Uninitialized texture received.");
      return;
    }
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, tex->m_texture);
  }

  void
  OGLGraphicsAPI::setTopology(TOPOLOGY::E topology) {
    switch (topology) {
    case TOPOLOGY::E::kPOINTS:
      m_topology = GL_POINTS;
      break;
    case TOPOLOGY::E::kLINES:
      m_topology = GL_LINES;
      break;
    case TOPOLOGY::E::kTRIANGLES:
      m_topology = GL_TRIANGLES;
      break;
    case TOPOLOGY::E::kLINE_STRIP:
      m_topology = GL_LINE_STRIP;
      break;
    case TOPOLOGY::E::kTRIANGLE_STRIP:
      m_topology = GL_TRIANGLE_STRIP;
      break;
    case TOPOLOGY::E::kLINE_ADJACENCY:
      m_topology = GL_LINES_ADJACENCY;
      break;
    case TOPOLOGY::E::kTRIANGLE_ADJANCENCY:
      m_topology = GL_TRIANGLES_ADJACENCY;
      break;
    case TOPOLOGY::E::kLINE_STRIP_ADJACENCY:
      m_topology = GL_LINE_STRIP_ADJACENCY;
      break;
    case TOPOLOGY::E::kTRIANGLE_STRIP_ADJACENCY:
      m_topology = GL_TRIANGLE_STRIP_ADJACENCY;
      break;
    }
  }

  void
  OGLGraphicsAPI::swapBuffer() {
    SwapBuffers(m_handle);
  }

  void
  OGLGraphicsAPI::resizeBackBuffer(uint32 width, uint32 height) {}

  void
  OGLGraphicsAPI::fillFormats() {
    m_formats.insert(std::make_pair(FORMATS::E::kR8_SNORM,
      std::make_pair(GL_R8_SNORM, GL_RED)));
    m_formats.insert(std::make_pair(FORMATS::E::kR16_SNORM,
      std::make_pair(GL_R16_SNORM, GL_RED)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG8_SNORM,
      std::make_pair(GL_RG8_SNORM, GL_RG)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG16_SNORM,
      std::make_pair(GL_RG16_SNORM, GL_RG)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGB10_A2UI,
      std::make_pair(GL_RGB10_A2UI, GL_RGBA)));
    m_formats.insert(std::make_pair(FORMATS::E::kR16_FLOAT,
      std::make_pair(GL_R16F, GL_RED)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG16_FLOAT,
      std::make_pair(GL_RG16F, GL_RG)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA16_FLOAT,
      std::make_pair(GL_RGBA16F, GL_RGBA)));
    m_formats.insert(std::make_pair(FORMATS::E::kR32_FLOAT,
      std::make_pair(GL_R32F, GL_RED)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG32_FLOAT,
      std::make_pair(GL_RG32F, GL_RG)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGB32_FLOAT,
      std::make_pair(GL_RGB32F, GL_RGB)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA32_FLOAT,
      std::make_pair(GL_RGBA32F, GL_RGBA)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG11B10_FLOAT,
      std::make_pair(GL_R11F_G11F_B10F, GL_RGB)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGB9_E5,
      std::make_pair(GL_RGB9_E5, GL_RGB)));
    m_formats.insert(std::make_pair(FORMATS::E::kR8_INT,
      std::make_pair(GL_R8I, GL_RED_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kR8_UINT,
      std::make_pair(GL_R8UI, GL_RED_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kR16_INT,
      std::make_pair(GL_R16I, GL_RED_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kR16_UINT,
      std::make_pair(GL_R16UI, GL_RED_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kR32_INT,
      std::make_pair(GL_R32I, GL_RED_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kR32_UINT,
      std::make_pair(GL_R32UI, GL_RED_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG8_INT,
      std::make_pair(GL_RG8I, GL_RG_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG8_UINT,
      std::make_pair(GL_RG8UI, GL_RG_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG16_INT,
      std::make_pair(GL_RG16I, GL_RG_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG16_UINT,
      std::make_pair(GL_RG16UI, GL_RG_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG32_INT,
      std::make_pair(GL_RG32I, GL_RG_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG32_UINT,
      std::make_pair(GL_RG32UI, GL_RG_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGB32_INT,
      std::make_pair(GL_RGB32I, GL_RGB_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGB32_UINT,
      std::make_pair(GL_RGB32UI, GL_RGB_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA8_INT,
      std::make_pair(GL_RGBA8I, GL_RGBA_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA8_UINT,
      std::make_pair(GL_RGBA8UI, GL_RGBA_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA16_INT,
      std::make_pair(GL_RGBA16I, GL_RGBA_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA16_UINT,
      std::make_pair(GL_RGBA16UI, GL_RGBA_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA32_INT,
      std::make_pair(GL_RGBA32I, GL_RGBA_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA32_UINT,
      std::make_pair(GL_RGBA32UI, GL_RGBA_INTEGER)));
    m_formats.insert(std::make_pair(FORMATS::E::kR8_UNORM,
      std::make_pair(GL_R8, GL_RED)));
    m_formats.insert(std::make_pair(FORMATS::E::kR16_UNORM,
      std::make_pair(GL_R16, GL_RED)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG8_UNORM,
      std::make_pair(GL_RG8, GL_RG)));
    m_formats.insert(std::make_pair(FORMATS::E::kRG16_UNORM,
      std::make_pair(GL_RG16, GL_RG)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGB5A1_UNORM,
      std::make_pair(GL_RGB5_A1, GL_RGB)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA8_UNORM,
      std::make_pair(GL_RGBA8, GL_RGBA)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGB10A2_UNORM,
      std::make_pair(GL_RGB10_A2, GL_RGBA)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA16_UNORM,
      std::make_pair(GL_RGBA16, GL_RGBA)));
    m_formats.insert(std::make_pair(FORMATS::E::kRGBA8_SRGB_UNORM,
      std::make_pair(GL_SRGB8_ALPHA8, GL_RGBA)));
    m_formats.insert(std::make_pair(FORMATS::E::kD24_S8,
      std::make_pair(GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL)));
  }
}