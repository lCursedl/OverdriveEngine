#include "ovGraphicsAPI.h"

namespace ovEngineSDK {
  GraphicsAPI& g_graphicsAPI() {
    return GraphicsAPI::instance();
  }
}