#include <ovDXBuffer.h>

namespace ovEngineSDK {
  DXBuffer::DXBuffer() {
    m_buffer = nullptr;
  }

  DXBuffer::~DXBuffer() {
    if (nullptr != m_buffer) {
      m_buffer->Release();
    }
  }
}