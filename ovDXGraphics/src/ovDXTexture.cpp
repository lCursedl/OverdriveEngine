#include <ovDXTexture.h>

namespace ovEngineSDK {
  DXTexture::DXTexture() {
    m_texture = nullptr;
    m_rtv = nullptr;
    m_dsv = nullptr;
    m_srv = nullptr;
    m_uav = nullptr;
  }

  DXTexture::~DXTexture() {
    if ( m_texture) {
      m_texture->Release();
    }
    if (m_rtv) {
      m_rtv->Release();
    }
    if ( m_dsv) {
      m_dsv->Release();
    }
    if (m_srv) {
      m_srv->Release();
    }
    if (m_uav) {
      m_uav->Release();
    }
  }
}