#pragma once
#include <ovPrerequisitesCore.h>
#include <ovModule.h>

namespace ovEngineSDK {
  
  class OV_CORE_EXPORT Random : public Module<Random>
  {
   public:
    Random();
    virtual ~Random() = default;

    int32
    GetRandom(int32 first, int32 second);

   protected:
    uint32 m_seed;
    std::default_random_engine m_generator;
  };
}
