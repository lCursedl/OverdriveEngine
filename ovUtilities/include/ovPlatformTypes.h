/*****************************************************************************/
#pragma once

/*****************************************************************************/
/**
 * Includes
 */
/*****************************************************************************/
#include <cstdint>
#include "ovPlatformDefines.h"

namespace ovEngineSDK {
  using std::uint8_t;
  using std::uint16_t;
  using std::uint32_t;
  using std::uint64_t;
  using std::int8_t;
  using std::int16_t;
  using std::int32_t;
  using std::int64_t;

  /***************************************************************************/
  /**
   * Basic unsigned types
   */
  /***************************************************************************/
  using uint8 = uint8_t;
  using uint16 = uint16_t;
  using uint32 = uint32_t;
  using uint64 = uint64_t;

  /***************************************************************************/
  /**
    * Basic signed types
   */
  /***************************************************************************/
  using int8 = int8_t;
  using int16 = int16_t;
  using int32 = int32_t;
  using int64 = int64_t;
  using SIZE_T = size_t;
}