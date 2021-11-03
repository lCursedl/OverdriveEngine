#include "ovDXComputeShader.h"

namespace ovEngineSDK {

  DXComputeShader::~DXComputeShader() {
    if (nullptr != m_blob) {
      m_blob->Release();
    }
    if (nullptr != m_cs) {
      m_cs->Release();
    }
  }
  DXComputeShader::DXComputeShader() {
    m_cs = NULL;
    m_blob = NULL;
  }
}