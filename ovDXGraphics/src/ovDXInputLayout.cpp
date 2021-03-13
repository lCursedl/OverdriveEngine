#include <ovDXInputLayout.h>

namespace ovEngineSDK {
  DXInputLayout::DXInputLayout() {
    m_InputLayout = nullptr;
  }

  DXInputLayout::~DXInputLayout() {
    if (nullptr != m_InputLayout) {
      m_InputLayout->Release();
    }
  }
}