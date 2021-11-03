#include <ovDXBuffer.h>

namespace ovEngineSDK {
  DXBuffer::DXBuffer() {
    m_buffer = nullptr;
    m_srv = nullptr;
    m_uav = nullptr;
  }

  DXBuffer::~DXBuffer() {
    if (nullptr != m_buffer) {
      m_buffer->Release();
    }
    if (m_srv) {
      m_srv->Release();
    }
    if (m_uav) {
      m_uav->Release();
    }
  }
}