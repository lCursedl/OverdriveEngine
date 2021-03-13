#pragma once
#include <ovShader.h>
#include <d3d11.h>

namespace ovEngineSDK {
  /**
  * @class       DXVertexShader
  * @brief       Class which holds info for a D3D11's vertex shader.
  */
  class DXVertexShader final : public VertexShader
  {
   protected:

    /**
    * @fn        DXVertexShader()
    * @brief     Class constructor.
    *            Sets m_blob & m_vs as nullptr.
    */
    DXVertexShader();

   public:

    /** 
    * @fn        ~DXVertexShader()
    * @brief     Class destructor.
    *            If m_blob or m_vs was used, release memory before deleting object.
    */
    ~DXVertexShader();

  private:

    ID3DBlob* m_blob;               /**< ID3D11Blob pointer */
    ID3D11VertexShader* m_vs;       /**< ID3D11VertexShader pointer */

    friend class DXGraphicsAPI;
  };
}