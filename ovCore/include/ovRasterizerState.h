#pragma once

namespace ovEngineSDK {

  /**
  * @enum        FILL_MODE
  * @brief       Enum which contains definitions for fill mode types
  */
  namespace FILL_MODE {
    enum E {
      kWIREFRAME = 0,
      kSOLID = 1
    };
  }
  /**
  * @enum        CULL_MODE
  * @brief       Enum which contains definitions for cull mode types
  */
  namespace CULL_MODE {
    enum E {
      kNONE = 0,
      kFRONT = 1,
      kBACK = 2
    };
  }
  /**
  * @class       Rasterizer State
  * @brief       Class which serves as an interface for both DXRasterizerState
                 and OGLRasterizerState.
                 Contains only a constructor and virtual destructor.
  */
  class RasterizerState
  {
   public:
    RasterizerState() = default;
    virtual ~RasterizerState() = default;
  };
}
