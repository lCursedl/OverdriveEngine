#pragma once
#include <ovPrerequisitesOGL.h>
#include <ovBuffer.h>

namespace ovEngineSDK {
  /**
  * @class       OGLBuffer
  * @brief       Class which holds info for an OpenGL buffer.
  */
  class OGLBuffer final : public Buffer
  {
   public:

    /** 
    * @fn        ~OGLBuffer()
    * @brief     Class destructor.
    *            If m_buffer was used, release memory before deleting object.
    */
    ~OGLBuffer();

   protected:

    /**
    * @fn        OGLBuffer()
    * @brief     Class constructor.
    */
    OGLBuffer() = default;
   private:

    uint32 m_type = 0;            /**< unsigned int for buffer type storing. */
    int32 m_size = 0;             /**< int for buffer size storing. */
    uint32 m_buffer = 0;          /**< unsigned int for OpenGL buffer index */

    friend class OGLGraphicsAPI;
  };
}