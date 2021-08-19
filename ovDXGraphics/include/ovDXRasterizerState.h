#pragma once
#include <ovPrerequisitesDX.h>
#include <ovRasterizerState.h>

namespace ovEngineSDK {
  /**
  * @class       DXRasterizerState
  * @brief       Class which holds info for a D3D11's rasterizer state.
  */
  class DXRasterizerState : public RasterizerState
  {
   public:
     /**
     * @fn        ~DXRasterizerState()
     * @brief     Class destructor.
     *            If m_rState was used, release memory before deleting object.
     */
    ~DXRasterizerState();

   protected:
    
     /**
     * @fn        DXRasterizerState()
     * @brief     Class constructor.
     *            Sets m_rState as nullptr.
     */
    DXRasterizerState();
   
   private:
    ID3D11RasterizerState* m_rState;    /**< ID3D11RasterizerState pointer */

    friend class DXGraphicsAPI;
  };
}
