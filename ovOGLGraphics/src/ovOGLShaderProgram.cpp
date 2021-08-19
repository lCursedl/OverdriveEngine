#include <ovOGLShaderProgram.h>
#include <ovOGLVertexShader.h>
#include <ovOGLPixelShader.h>

#if OV_PLATFORM == OV_PLATFORM_WIN32
#include <Windows.h>
#endif // OV_PLATFORM == OV_PLATFORM_WIN32

namespace ovEngineSDK {
  OGLShaderProgram::~OGLShaderProgram() {
    if (0 != m_program) {
      glDeleteProgram(m_program);
    }
  }

  void OGLShaderProgram::setVertexShader(SPtr<VertexShader> vertexShader) {
    if (m_program == 0 || vertexShader == nullptr) {
      return;
    }
    auto vs = static_pointer_cast<OGLVertexShader>(vertexShader);
    if (vs->m_vertexShader == 0) {
      return;
    }
    glAttachShader(m_program, vs->m_vertexShader);
    m_pVertexShader = vertexShader;
  }

  void OGLShaderProgram::setPixelShader(SPtr<PixelShader> pixelShader) {
    if (m_program == 0 || pixelShader == nullptr) {
      return;
    }
    auto ps = static_pointer_cast<OGLPixelShader>(pixelShader);
    if (ps->m_ps == 0) {
      return;
    }
    glAttachShader(m_program, ps->m_ps);
    m_pPixelShader = pixelShader;
  }

  void OGLShaderProgram::linkProgram() {
    glLinkProgram(m_program);
    int result;
    char log[512];
    glGetProgramiv(m_program, GL_LINK_STATUS, &result);

    if (!result)
    {
      glGetProgramInfoLog(m_program, 512, 0, log);
      OutputDebugStringA(log);
    }
    glDetachShader(m_program,
      static_pointer_cast<OGLVertexShader>(m_pVertexShader)->m_vertexShader);
    glDetachShader(m_program,
      static_pointer_cast<OGLPixelShader>(m_pPixelShader)->m_ps);
  }
}