#pragma once
#include <ovPrerequisitesOGL.h>
#include <ovShader.h>

namespace ovEngineSDK {
  /**
  * @class       DXPixelShader
  * @brief       Class which holds info for a D3D11's pixel shader.
   */
  class OGLPixelShader final : public PixelShader
  {
   public:

    /**
    * @fn        ~OGLPixelShader()
    * @brief     Class destructor.
    *            If m_ps was used, release memory before deleting object.
    */
    ~OGLPixelShader();

  protected:

    /**
    * @fn        OGLPixelShader()
    * @brief     Class constructor.
    */
    OGLPixelShader() = default;

  private:
    uint32 m_ps = 0;          /**< unsigned int for OpenGL pixel shader index */

    friend class OGLGraphicsAPI;
    friend class OGLShaderProgram;
  };
}