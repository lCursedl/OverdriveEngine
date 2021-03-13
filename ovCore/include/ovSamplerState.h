#pragma once

namespace ovEngineSDK {
  
  /**
  * @enum        FILTER_LEVEL
  *	@brief       Enum which contains definitions for different filter levels.
  */
  namespace FILTER_LEVEL {
    enum E {
      FILTER_POINT = 0,
      FILTER_LINEAR
    };
  }

  /**
  * @enum        WRAPPING
  *	@brief       Enum which contains definitions for different wrapping modes.
  */
  namespace WRAPPING {
    enum E {
      WRAP = 1,
      MIRROR,
      CLAMP ,
      BORDER,
      MIRROR_ONCE
    };
  }

  /**
  * @enum        COMPARISON
  *	@brief       Enum which contains definitions for different comparison modes.
  */
  namespace COMPARISON {
    enum E {
      NEVER = 1,
      LESS,
      EQUAL,
      LESS_EQUAL,
      GREATER,
      NOT_EQUAL,
      GREATER_EQUAL,
      ALWAYS
    };
  }

  /**
  * @class       CSamplerState
  *	@brief       Class which serves as an interface for both
  *              CDXSamplerState & COGLSamplerState. Contains only a constructor
  *              and virtual destructor
  */
  class SamplerState
  {
  public:
    SamplerState() = default;
    virtual ~SamplerState() = default;
  };
}