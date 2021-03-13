#include <ovOGLInputLayout.h>

namespace ovEngineSDK {
  OGLInputLayout::~OGLInputLayout() {
    if (0 != m_vao) {
      glDeleteVertexArrays(1, &m_vao);
    }
  }
}