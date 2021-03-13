#pragma once
#include <ovSamplerState.h>
#include <d3d11.h>

namespace ovEngineSDK {
  /**
  * @class       DXSamplerState
  * @brief       Class which holds info for a D3D11's sampler state.
  */
  class DXSamplerState final : public SamplerState
  {
   protected:

    /**
    * @fn        DXSamplerState()
    * @brief     Class constructor.
    *            Sets m_sampler as nullptr.
    */
    DXSamplerState();

   public:

    /**
    * @fn        ~DXSamplerState()
    * @brief     Class destructor.
    *            If m_sampler was used, release memory before deleting object.
    */
    ~DXSamplerState();

   private:

    ID3D11SamplerState* m_sampler;      /**< ID3D11SamplerState pointer. */

    friend class DXGraphicsAPI;
  };
}