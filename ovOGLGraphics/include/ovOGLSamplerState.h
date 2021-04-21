#pragma once
#include <ovPrerequisitesOGL.h>
#include <ovSamplerState.h>

namespace ovEngineSDK {
  /**
  * @class       OGLSamplerState
  * @brief       Class which holds info for an OpenGL sampler object.
  */
  class OGLSamplerState final : public SamplerState
  {
   protected:

    /**
    * @fn        DXSamplerState()
    * @brief     Class constructor.
    */
     OGLSamplerState() = default;

   public:

    /**
    * @fn        ~DXSamplerState()
    * @brief     Class destructor.
    *            If m_sampler was used, release memory before deleting object.
    */
    ~OGLSamplerState();

   private:

    uint32 m_sampler = 0;      /**< ID3D11SamplerState pointer. */

    friend class OGLGraphicsAPI;
  };
}