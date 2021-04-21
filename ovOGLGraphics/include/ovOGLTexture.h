#pragma once
#include <ovPrerequisitesOGL.h>
#include <ovTexture.h>

namespace ovEngineSDK {
  /**
  * @class       OGLTexture
  *	@brief       Class which holds info for an OpenGL texture.
  */
  class OGLTexture final : public Texture
  {
   protected:

    /**
    * @fn        OGLTexture()
    *	@brief     Class constructor.
    */
    OGLTexture() = default;

   public:

    /**
    * @fn        ~OGLTexture()
    *	@brief     Class destructor.
    *            if m_texture or m_framebuffer was used, release memory 
    *	           before deleting object.
    */
    ~OGLTexture();

  private:

    uint32 m_texture = 0;        /**< unsigned int for OpenGL texture index */
    uint32 m_framebuffer = 0;    /**< unsigned int for OpenGL framebuffer index */

    friend class OGLGraphicsAPI;
  };
}