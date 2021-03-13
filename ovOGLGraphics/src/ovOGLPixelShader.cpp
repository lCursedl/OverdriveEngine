#include <ovOGLPixelShader.h>

namespace ovEngineSDK {
  OGLPixelShader::~OGLPixelShader() {
    if (0 != m_ps) {
      glDeleteShader(m_ps);
    }
  }
}