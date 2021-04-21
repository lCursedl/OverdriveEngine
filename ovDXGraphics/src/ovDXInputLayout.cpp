#include <ovDXInputLayout.h>

namespace ovEngineSDK {
  DXInputLayout::DXInputLayout() {
    m_inputLayout = nullptr;
  }

  DXInputLayout::~DXInputLayout() {
    if (nullptr != m_inputLayout) {
      m_inputLayout->Release();
    }
  }
}