#pragma once
#include <ovBuffer.h>
#include <d3d11.h>

namespace ovEngineSDK {
  /**
  * @class       CDXBuffer
  * @brief       Class which holds info for a D3D11's buffer.
  */
  class DXBuffer final : public Buffer
  {
   public:

    /** 
    * @fn        ~CDXBuffer()
    * @brief     Class destructor.
    *            If m_Buffer was used, release memory before deleting object.
    */
    ~DXBuffer();

   protected:

    /**
    * @fn        CDXBuffer()
    * @brief     Class constructor.
    *            Sets m_Buffer as nullptr.
    */
    DXBuffer();
   private:

    ID3D11Buffer* m_buffer;     /**< ID3D11Buffer pointer */

    friend class DXGraphicsAPI;
  };
}