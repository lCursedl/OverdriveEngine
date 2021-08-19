#include "ovDXRasterizerState.h"

namespace ovEngineSDK {
  DXRasterizerState::DXRasterizerState() {
    m_rState = nullptr;
  }
  DXRasterizerState::~DXRasterizerState() {
    if (nullptr != m_rState) {
      m_rState->Release();
    }
  }  
}
