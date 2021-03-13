#include <ovOGLShaderProgram.h>
#include <ovOGLVertexShader.h>
#include <ovOGLPixelShader.h>

namespace ovEngineSDK {
  OGLShaderProgram::~OGLShaderProgram() {
    if (0 != m_program) {
      glDeleteProgram(m_program);
    }
  }

  void OGLShaderProgram::setVertexShader(VertexShader* vertexShader) {
    if (m_program == 0) {
      return;
    }

    OGLVertexShader* vs = dynamic_cast<OGLVertexShader*>(vertexShader);

    if (!vs) {
      return;
    }

    if (vs->m_vertexShader == 0) {
      return;
    }
    glAttachShader(m_program, vs->m_vertexShader);
    m_pVertexShader = vertexShader;
  }

  void OGLShaderProgram::setPixelShader(PixelShader* pixelShader) {
    if (m_program == 0) {
      return;
    }

    OGLPixelShader* ps = dynamic_cast<OGLPixelShader*>(pixelShader);

    if (!ps) {
      return;
    }

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
    }
    glDetachShader(m_program,
      dynamic_cast<OGLVertexShader*>(m_pVertexShader)->m_vertexShader);
    glDetachShader(m_program,
      dynamic_cast<OGLPixelShader*>(m_pPixelShader)->m_ps);
  }
}