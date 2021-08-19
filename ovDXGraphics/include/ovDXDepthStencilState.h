#pragma once
#include <ovDepthStencilState.h>
#include <ovPrerequisitesDX.h>

namespace ovEngineSDK {
  /**
  * @class       DXDepthStencilState
  * @brief       Class which holds info for a D3D11's depth stencil state.
  */
  class DXDepthStencilState final : public DepthStencilState
  {
  protected:

    /**
    * @fn        DXDepthStencilState()
    * @brief     Class constructor.
    *            Sets m_depthState as nullptr.
    */
    DXDepthStencilState();

  public:

    /**
    * @fn        ~DXDepthStencilState()
    * @brief     Class destructor.
    *            If m_depthState was used, release memory before deleting object.
    */
    ~DXDepthStencilState();

  private:

    ID3D11DepthStencilState* m_depthState;      /**< ID3D11DepthStencilState pointer. */

    friend class DXGraphicsAPI;
  };
}
