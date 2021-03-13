#include <ovDXVertexShader.h>

namespace ovEngineSDK {
  DXVertexShader::DXVertexShader() {
    m_blob = NULL;
    m_vs = NULL;
  }

  DXVertexShader::~DXVertexShader() {
    if (nullptr !=m_blob) {
      m_blob->Release();
    }
    if (nullptr != m_vs) {
      m_vs->Release();
    }
  }
}