#pragma once

#include "ovPlatformDefines.h"
#include "ovPlatformTypes.h"

namespace ovEngineSDK {
  
  class OV_UTILITY_EXPORT Math
  {
   public:
     static int32
     trunc(float F) {
       return static_cast<int32>(F);
     }

     static float
     truncFloat(float F) {
       return static_cast<float>(trunc(F));
     }

     
  };
}