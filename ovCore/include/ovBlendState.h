#pragma once

namespace ovEngineSDK {
  
  namespace BLEND_TYPE {
    enum E {
      ZERO = 1,
      ONE,
      SRC_COLOR,
      INV_SRC_COLOR,
      SRC_ALPHA,
      INV_SRC_ALPHA,
      DEST_ALPHA,
      INV_DEST_ALPHA,
      DEST_COLOR,
      INV_DEST_COLOR,
      SRC_ALPHA_SAT,
      BLEND_FACTOR = 14,
      INV_BLEND_FACTOR,
      SRC1_COLOR,
      INV_SRC1_COLOR,
      SRC1_APLHA,
      INV_SRC1_ALPHA
    };
  }

  namespace BLEND_OP {
    enum E {
      ADD = 1,
      SUBTRACT,
      REV_SUBTRACT,
      MIN,
      MAX
    };
  }

  /**
  * @class       BlendState
  * @brief       Class which serves as an interface for both DXBlendState and
  *              OGLBlendState.
  *              Contains only a constructor and virtual destructor
  */
  class BlendState
  {
   public:
    BlendState() = default;
    virtual ~BlendState() = default;
  };
}