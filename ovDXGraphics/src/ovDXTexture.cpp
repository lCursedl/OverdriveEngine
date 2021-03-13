#include <ovDXTexture.h>

namespace ovEngineSDK {
  DXTexture::DXTexture() {
    m_texture = nullptr;
    m_rtv = nullptr;
    m_dsv = nullptr;
    m_srv = nullptr;
  }

  DXTexture::~DXTexture() {
    if (nullptr != m_texture) {
      m_texture->Release();
    }
    if (nullptr != m_rtv) {
      m_rtv->Release();
    }
    if (nullptr != m_dsv) {
      m_dsv->Release();
    }
    if (nullptr != m_srv) {
      m_srv->Release();
    }
  }
}