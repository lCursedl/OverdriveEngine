#pragma once
#include <ovShader.h>
#include <d3d11.h>

namespace ovEngineSDK {
  /**
  * @class       DXPixelShader
  * @brief       Class which holds info for a D3D11's pixel shader.
   */
  class DXPixelShader final : public PixelShader
  {
   public:

    /**
    * @fn        ~DXPixelShader()
    * @brief     Class destructor.
    *            If m_PS or m_Blob was used, release memory before deleting object.
    */
    ~DXPixelShader();

  protected:

    /**
    * @fn        DXPixelShader()
    * @brief     Class constructor.
    *            Sets m_PS & m_Blob as nullptr.
    */
    DXPixelShader();

  private:
    ID3D11PixelShader* m_ps;        /**< ID3D11PixelShader pointer */
    ID3DBlob* m_blob;               /**< ID3D11Blob pointer */

    friend class DXGraphicsAPI;
  };
}