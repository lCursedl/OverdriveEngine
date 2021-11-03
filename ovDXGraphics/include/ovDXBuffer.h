#pragma once
#include <ovPrerequisitesDX.h>
#include <ovBuffer.h>

namespace ovEngineSDK {
  /**
  * @class       DXBuffer
  * @brief       Class which holds info for a D3D11's buffer.
  */
  class DXBuffer final : public Buffer
  {
   public:

    /** 
    * @fn        ~DXBuffer()
    * @brief     Class destructor.
    *            If m_buffer was used, release memory before deleting object.
    */
    ~DXBuffer();

   protected:

    /**
    * @fn        DXBuffer()
    * @brief     Class constructor.
    *            Sets m_buffer as nullptr.
    */
    DXBuffer();
   private:

    ID3D11Buffer* m_buffer;     /**< ID3D11Buffer pointer */
    ID3D11ShaderResourceView* m_srv;
    ID3D11UnorderedAccessView* m_uav;

    friend class DXGraphicsAPI;
  };
}