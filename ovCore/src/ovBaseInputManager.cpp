#include "ovBaseInputManager.h"

namespace ovEngineSDK {
  OV_CORE_EXPORT BaseInputManager& g_baseInput() {
    return BaseInputManager::instance();
  }
}