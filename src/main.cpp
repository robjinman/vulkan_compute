#include "gpu.hpp"
#include "types.hpp"
#include <cstdlib>
#include <chrono>
#include <iostream>

// Type of buffers
//
// Uniforms buffers (UBOs) for small, fixed-size read-only buffers
// SSBO
// Push constants - Small, frequently udpated data - more efficient than UBOs
//
// Flags determine which heap buffer is allocated from:
// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT - Fast, device local memory
// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT - For frequently updated input data
// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT - Host memory visible from GPU, good for staging buffers and UBOs
// 

/*
Small, read-only, frequent inputs
- Push constants
- UBDs
Large, frequent inputs, don't need staging buffer
- SSBO - VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
Large, device local, need staging buffer
- VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT

*/

void printBuffer(const std::vector<netfloat_t>& buffer) {
  for (size_t i = 0; i < buffer.size(); ++i) {
    std::cout << buffer[i] << " ";
  }
  std::cout << std::endl;
}

int main() {
  GpuPtr gpu = createGpu();

  std::vector<netfloat_t> bufferAData{
    1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8
  };

  std::vector<netfloat_t> bufferBData(bufferAData.size());

  BufferHandle bufferA = gpu->allocateBuffer(bufferAData.size() * sizeof(netfloat_t), 0);
  BufferHandle bufferB = gpu->allocateBuffer(bufferBData.size() * sizeof(netfloat_t), 0);

  ShaderHandle shader1 = gpu->compileShader("shaders/shader.glsl", { bufferA, bufferB },
    { 16, 1, 1 });

  ShaderHandle shader2 = gpu->compileShader("shaders/shader2.glsl", { bufferB, bufferA },
    { 16, 1, 1 });

  auto startTime = std::chrono::high_resolution_clock::now();

  gpu->submitBufferData(bufferA, bufferAData.data());

  gpu->queueShader(shader1);
  gpu->queueShader(shader2);

  gpu->flushQueue();

  gpu->retrieveBuffer(bufferA, bufferAData.data());
  gpu->retrieveBuffer(bufferB, bufferBData.data());

  auto endTime = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

  printBuffer(bufferAData);
  printBuffer(bufferBData);

  std::cout << "Time elapsed: " << time << " microseconds" << std::endl;

  return EXIT_SUCCESS;
}
