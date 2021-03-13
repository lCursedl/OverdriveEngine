#include <ovOGLBuffer.h>

namespace ovEngineSDK {

  OGLBuffer::~OGLBuffer() {
    if (0 != m_buffer) {
      glDeleteBuffers(1, &m_buffer);
    }
  }
}