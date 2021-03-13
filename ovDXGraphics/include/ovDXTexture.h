#pragma once
#include <ovTexture.h>
#include <d3d11.h>

namespace ovEngineSDK {
  /**
  * @class       DXTexture
  *	@brief       Class which holds info for a D3D11's texture.
  */
  class DXTexture final : public Texture
  {
   protected:

    /**
    * @fn        DXTexture()
    *	@brief     Class constructor.
    *            Sets m_pTexture, m_pRTV, m_pDSV and m_pSRV as nullptr;
    */
    DXTexture();

   public:

    /**
    * @fn        ~DXTexture()
    *	@brief     Class destructor.
    *            If m_pTexture, m_pRTV, m_pDSV or m_pSRV was used, release memory before
    *	           deleting object.
    */
    ~DXTexture();

  private:

    ID3D11Texture2D* m_texture;	/**< ID3D11Texture2D pointer */
    ID3D11RenderTargetView* m_rtv;		/**< ID3D11RenderTargetView pointer */
    ID3D11DepthStencilView* m_dsv;		/**< ID3D11DepthStencilView pointer */
    ID3D11ShaderResourceView* m_srv;		/**< ID3D11ShaderResourceView pointer */

    friend class DXGraphicsAPI;
  };
}