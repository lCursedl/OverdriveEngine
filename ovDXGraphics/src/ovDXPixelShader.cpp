#include <ovDXPixelShader.h>

namespace ovEngineSDK {
  DXPixelShader::DXPixelShader() {
    m_ps = nullptr;
    m_blob = nullptr;
  }

  DXPixelShader::~DXPixelShader() {
    if (nullptr != m_ps) {
      m_ps->Release();
    }
    if (nullptr != m_blob) {
      m_blob->Release();
    }
  }
}