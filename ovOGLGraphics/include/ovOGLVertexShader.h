#pragma once
#include <ovPrerequisitesOGL.h>
#include <ovShader.h>

namespace ovEngineSDK {
  /**
  * @class       DXVertexShader
  * @brief       Class which holds info for a OpenGL vertex shader
  */
  class OGLVertexShader final : public VertexShader
  {
   protected:

    /**
    * @fn        OGLVertexShader()
    * @brief     Class constructor.
    */
    OGLVertexShader() = default;

   public:

    /** 
    * @fn        ~OGLVertexShader()
    * @brief     Class destructor.
    *            If m_vertexShader was used, release memory before deleting object.
    */
    ~OGLVertexShader();

  private:

    uint32 m_vertexShader = 0;    /**< unsigned int for OpenGL vertex shader index. */

    friend class OGLGraphicsAPI;
    friend class OGLShaderProgram;
  };
}