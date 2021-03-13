#include <ovOGLVertexShader.h>

namespace ovEngineSDK {

  OGLVertexShader::~OGLVertexShader() {
    if (0 != m_vertexShader) {
      glDeleteShader(m_vertexShader);
    }
  }
}