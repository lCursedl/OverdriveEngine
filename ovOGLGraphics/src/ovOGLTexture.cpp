#include <ovOGLTexture.h>

namespace ovEngineSDK {
  OGLTexture::~OGLTexture() {
    if (m_texture != 0) {
      glDeleteTextures(1, &m_texture);
    }
    if (m_framebuffer != 0) {
      glDeleteFramebuffers(1, &m_framebuffer);
    }
  }
}