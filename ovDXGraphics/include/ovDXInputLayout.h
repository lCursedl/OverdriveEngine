#pragma once
#include <ovInputLayout.h>
#include <ovPrerequisitesDX.h>

namespace ovEngineSDK {
  /**
  * @class       XInputLayout
  * @brief       Class which holds info for a D3D11's input layout.
  */
  class DXInputLayout final : public InputLayout
  {
   public:

    /**
    * @fn        ~DXInputLayout()
    * @brief     Class destructor. 
    *
    *            If m_inputLayout was used, release memory before deleting object.
    */
    ~DXInputLayout();

   protected:

    /** 
    * @fn        DXInputLayout()
    * @brief     Class constructor. 
    *
    *            Sets m_inputLayout as nullptr.
    */
    DXInputLayout();

  private:

    ID3D11InputLayout* m_inputLayout;   /**< ID3D11InputLayout pointer */

    friend class DXGraphicsAPI;
  };
}