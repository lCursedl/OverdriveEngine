#include <ovOGLSamplerState.h>

namespace ovEngineSDK {
  OGLSamplerState::~OGLSamplerState() {
    if (0 != m_sampler) {
      glDeleteSamplers(1, &m_sampler);
    }    
  }
}