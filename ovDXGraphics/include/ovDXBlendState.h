#pragma once
#include <ovPrerequisitesDX.h>
#include <ovBlendState.h>
#include <ovVector4.h>

namespace ovEngineSDK {
  /**
  * @class       DXBlendState
  * @brief       Class which holds info for a D3D11's blend state.
  */
  class DXBlendState : public BlendState
  {
   public:
    
    /**
    * @fn        ~DXBlendState()
    * @brief     Class destructor.
    *            If m_bs was used, release memory before deleting object.
    */
    ~DXBlendState();

   protected:

   /**
   * @fn         DXBlendState()
   * @brief      Class constructor.
   *             Sets m_bs as nullptr.
   */
    DXBlendState() = default;

    ID3D11BlendState* m_bs = nullptr;   /**< ID3D11BlendState pointer */
    Vector4 m_blendFactor;

    friend class DXGraphicsAPI;
  };
}