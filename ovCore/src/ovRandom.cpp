#include "ovRandom.h"

namespace ovEngineSDK {
  
  Random::Random() {
    auto now = std::chrono::system_clock::now();
    auto tMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    m_seed = static_cast<uint32>(tMilliseconds);
    m_generator.seed(m_seed);
  }

  int32
  ovEngineSDK::Random::GetRandom(int32 first, int32 second) {
    std::uniform_int_distribution<int32> distribution(first, second);
    return distribution(m_generator);
  }
}
