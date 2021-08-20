#include "ovBaseRenderer.h"

namespace ovEngineSDK {
  OV_CORE_EXPORT BaseRenderer& g_baseRenderer() {
    return BaseRenderer::instance();
  }
}