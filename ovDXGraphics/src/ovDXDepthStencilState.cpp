#include "ovDXDepthStencilState.h"

namespace ovEngineSDK {
  DXDepthStencilState::DXDepthStencilState() {
    m_depthState = nullptr;
  }
  DXDepthStencilState::~DXDepthStencilState() {
    if (nullptr != m_depthState) {
      m_depthState->Release();
    }
  }
}
