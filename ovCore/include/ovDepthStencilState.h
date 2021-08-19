#pragma once

namespace ovEngineSDK {
  /**
  * @class       Depth Stencil State
  * @brief       Class which serves as an interface for both DXDepthStencilState
                 and OGLDepthStencilState.
                 Contains only a constructor and virtual destructor.
  */
  class DepthStencilState
  {
   public:
    DepthStencilState() = default;
    virtual ~DepthStencilState() = default;
  };
}