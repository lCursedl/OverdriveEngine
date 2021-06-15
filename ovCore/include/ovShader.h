#pragma once

namespace ovEngineSDK {
  /**
  * @enum        SHADER_TYPE
  *	@brief       Enum which contains definitions for different shader types
   */
  namespace SHADER_TYPE {
    enum E {
      VERTEX_SHADER = 0,
      PIXEL_SHADER
    };
  }

  /**
  * @class       VertexShader
  *	@brief       Class which serves as an interface for both DXVertexShader & OGLVertexShader.
  *
  *	             Contains only a constructor and virtual destructor.
  */
  class VertexShader
  {
   public:
    VertexShader() = default;
    virtual ~VertexShader() = default;
  };

  /**
  * @class       PixelShader
  *	@brief       Class which serves as an interface for both DXPixelShader & OGLPixelShader.
  *
  *	             Contains only a constructor and virtual destructor.
  */
  class PixelShader
  {
  public:
    PixelShader() = default;
    virtual ~PixelShader() = default;
  };
}