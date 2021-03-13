/*****************************************************************************/
/**
* @file    ovFormats.h
* @author  Gustavo Alvarez (idv18c.galvarez@uartesdigitales.edu.mx)
* @date    2021/03/10
* @brief   File for common texture formats in DX and OGL
*/
/*****************************************************************************/
#pragma once

namespace ovEngineSDK {
  namespace FORMATS {
    enum E {
      R8_UNORM,
      R8_SNORM,
      R16_UNORM,
      R16_SNORM,
      RG8_UNORM,
      RG8_SNORM,
      RG16_UNORM,
      RG16_SNORM,
      RGB10_A2UI,
      R16_FLOAT,
      RG16_FLOAT,
      RGBA16_FLOAT,
      R32_FLOAT,
      RG32_FLOAT,
      RGB32_FLOAT,
      RGBA32_FLOAT,
      RG11B10_FLOAT,
      RGB9_E5,
      R8_INT,
      R8_UINT,
      R16_INT,
      R16_UINT,
      R32_INT,
      R32_UINT,
      RG8_INT,
      RG8_UINT,
      RG16_INT,
      RG16_UINT,
      RG32_INT,
      RG32_UINT,
      RGB32_INT,
      RGB32_UINT,
      RGBA8_INT,
      RGBA8_UINT,
      RGBA16_INT,
      RGBA16_UINT,
      RGBA32_INT,
      RGBA32_UINT,
      RGB5A1_UNORM,
      RGBA8_UNORM,
      RGB10A2_UNORM,
      RGBA16_UNORM,
      RGBA8_SRGB_UNORM,
      D24_S8
    };
  }
}