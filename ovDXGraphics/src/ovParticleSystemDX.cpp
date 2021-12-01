#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "ovParticleSystemDX.h"

namespace ovEngineSDK {
  
  __global__ void
  checkParticles(float* lifeTime, bool* alive) {
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    alive[index] = lifeTime[index] > 0.0f;
  }
  
  void
  ParticleSystemDX::init() {
    
  }

  void
  ParticleSystemDX::update(float delta) {
    
  }

  void
  ParticleSystemDX::destroy() {
    
  }
}