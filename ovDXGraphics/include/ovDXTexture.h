#pragma once
#include <ovTexture.h>
#include <ovPrerequisitesDX.h>

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
    *            Sets m_texture, m_rtv, m_dsv and m_srv as nullptr;
    */
    DXTexture();

   public:

    /**
    * @fn        ~DXTexture()
    *	@brief     Class destructor. 
    *            If m_texture, m_rtv, m_dsv or m_srv was used, release memory before
    *	           deleting object.
    */
    ~DXTexture();

  private:

    ID3D11Texture2D* m_texture;	/**< ID3D11Texture2D pointer */
    ID3D11RenderTargetView* m_rtv;		/**< ID3D11RenderTargetView pointer */
    ID3D11DepthStencilView* m_dsv;		/**< ID3D11DepthStencilView pointer */
    ID3D11ShaderResourceView* m_srv;		/**< ID3D11ShaderResourceView pointer */
    ID3D11UnorderedAccessView* m_uav;

    friend class DXGraphicsAPI;
  };
}