#pragma once
#include <ovShaderProgram.h>
#include <ovDXVertexShader.h>
#include <ovDXPixelShader.h>

namespace ovEngineSDK {
  /**
  * @class       DXShaderProgram
  * @brief       Class which instantiates the Vertex and Pixel shader pointers.
  *              Because D3D11 doesn't have the concept of ShaderProgram, functionality
  *              of this class is limited
  */
  class DXShaderProgram :
    public ShaderProgram
  {
  protected:

    /** 
    * @fn        DXShaderProgram()
    * @brief     Class constructor.
    *            Initializes m_pVertexShader & m_pPixelShader.
    */
    DXShaderProgram() = default;

    friend class DXGraphicsAPI;
  };
}