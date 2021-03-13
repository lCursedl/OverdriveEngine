#pragma once
#include <ovShaderProgram.h>
#include <glad/glad.h>
#include <ovPrerequisitesUtil.h>

namespace ovEngineSDK {
  /**
  * @class       OGLShaderProgram
  * @brief       Class which holds info for an OpenGL shader program.
  *              The concept of ShaderProgram exists only on OGl, therefore the functionality
  *              of the base class is expanded here.
  */
  class OGLShaderProgram :
    public ShaderProgram
  {
   protected:

    /** 
    * @fn        OGLShaderProgram()
    * @brief     Class constructor.
    */
    OGLShaderProgram() = default;

   public:
    /**
    * @fn       ~OGLShaderProgram()
    * @brief    Class destructor.
    *           If m_program was used, release memory before deleting object.
    */
    ~OGLShaderProgram();

    void setVertexShader(VertexShader* vertexShader)                 override;

    void setPixelShader(PixelShader* pixelShader)                    override;

    void linkProgram()                                               override;

   private:
    
    uint32 m_program = 0; /**< unsigned int for OpenGL shader program index. */

    friend class OGLGraphicsAPI;
  };
}