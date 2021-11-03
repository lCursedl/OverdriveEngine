/*****************************************************************************/
/**
* @file    ovTexture.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/03/08
* @brief   Class for defining a base texture object.
*/
/*****************************************************************************/
#pragma once
#include <ovFormats.h>

namespace ovEngineSDK {
  /**
  *@enum         TEXTURE_BINDINGS
  *@brief        Enum which contains definitions for different texture bindings.
  */
  namespace TEXTURE_BINDINGS {
    enum class E {
      SHADER_RESOURCE = 0x01,
      RENDER_TARGET = 0x02,
      DEPTH_STENCIL = 0x04,
      UNORDERED_ACCESS = 0x08
    };
  }

  /**
  * @class       Texture
  *	@brief       Class which serves as an interface for both CDXTexture & COGLTexture.
  *
  *	             Contains only a constructor and virtual destructor.
  */
  class Texture
  {
   public:
    Texture() = default;
    virtual ~Texture() = default;
  };

  inline TEXTURE_BINDINGS::E operator | (TEXTURE_BINDINGS::E a,
                                  TEXTURE_BINDINGS::E b) {
    return static_cast<TEXTURE_BINDINGS::E>(static_cast<int>(a) | static_cast<int>(b));
  }

  inline bool operator & (TEXTURE_BINDINGS::E a, TEXTURE_BINDINGS::E b) {
    return static_cast<int>(a) & static_cast<int>(b);
  }

  inline TEXTURE_BINDINGS::E operator |= (TEXTURE_BINDINGS::E& a,
                                   TEXTURE_BINDINGS::E& b) {
    a = static_cast<TEXTURE_BINDINGS::E>(static_cast<int>(a) | static_cast<int>(b));
  }
}