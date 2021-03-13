#include <ovDXSamplerState.h>

namespace ovEngineSDK {
  DXSamplerState::DXSamplerState() {
    m_sampler = nullptr;
  }

  DXSamplerState::~DXSamplerState() {
    if (nullptr != m_sampler) {
      m_sampler->Release();
    }
  }
}