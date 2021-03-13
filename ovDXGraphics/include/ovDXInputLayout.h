#pragma once
#include <ovInputLayout.h>
#include <d3d11.h>

namespace ovEngineSDK {
  /**
  * @class       DXInputLayout
  * @brief       Class which holds info for a D3D11's input layout.
  */
  class DXInputLayout final : public InputLayout
  {
   public:

    /**
    * @fn        ~CDXInputLayout()
    * @brief     Class destructor.
    *
    *            If m_InputLayout was used, release memory before deleting object.
    */
    ~DXInputLayout();

   protected:

    /** 
    * @fn        DXInputLayout()
    * @brief     Class constructor.
    *
    *            Sets m_InputLayout as nullptr.
    */
    DXInputLayout();

  private:

    ID3D11InputLayout* m_InputLayout;   /**< ID3D11InputLayout pointer */

    friend class DXGraphicsAPI;
  };
}