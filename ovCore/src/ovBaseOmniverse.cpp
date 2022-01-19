#include "ovBaseOmniverse.h"

namespace ovEngineSDK {
  OV_CORE_EXPORT BaseOmniverse& g_baseOmniverse() {
    return BaseOmniverse::instance();
  }
}