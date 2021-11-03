#include "ovDXBlendState.h"

namespace ovEngineSDK {
  DXBlendState::~DXBlendState() {
    if (m_bs) {
      m_bs->Release();
    }
  }
}
