#pragma once

namespace ovEngineSDK {
  /**
  * @enum        BUFFER_TYPE
  * @brief       Enum which contains definitions for buffer types.
  */
  namespace BUFFER_TYPE {
    enum E {
      VERTEX_BUFFER = 0x1L,	/**< Vertex buffer enum value (1)*/
      INDEX_BUFFER = 0x2L,	/**< Index buffer enum value (2)*/
      CONST_BUFFER = 0x4L		/**< Constant buffer enum value (4)*/
    };
  }

  /**
  * @class       CBuffer
  *	@brief       Class which serves as an interface for both CDXBuffer and COGLBuffer.
  *
  *	             Contains only a constructor and virtual destructor.
  */
  class Buffer
  {
   public:
    Buffer() = default;
    virtual ~Buffer() = default;
  };
}