#include "gpu.hpp"
#include "types.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iostream>

std::string loadFile(const std::string& path) {
  std::ifstream fin(path);
  std::stringstream ss;
  std::string line;
  while (std::getline(fin, line)) {
    ss << line << std::endl;
  }
  return ss.str();
}

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

int main() {
  GpuPtr gpu = createGpu();

  std::vector<netfloat_t> data{
    1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8,   // Inputs
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    // Outputs
  };

  std::string shader1Source = loadFile("shaders/shader.glsl");
  ShaderHandle shader1 = gpu->compileShader(shader1Source, { 16, 1, 1 });

  std::string shader2Source = loadFile("shaders/shader2.glsl");
  ShaderHandle shader2 = gpu->compileShader(shader2Source, { 16, 1, 1 });

  auto startTime = std::chrono::high_resolution_clock::now();

  gpu->submitBuffer(data.data(), data.size() * sizeof(netfloat_t));

  gpu->queueShader(shader1);
  gpu->queueShader(shader2);

  gpu->flushQueue();

  gpu->retrieveBuffer(data.data());

  auto endTime = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

  for (size_t i = 0; i < data.size(); ++i) {
    std::cout << data[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "Time elapsed: " << time << " microseconds" << std::endl;

  return EXIT_SUCCESS;
}

// 4 8 12 16 20 24 28 32 4 8 12 16 20 24 28 32 2 4 6 8 10 12 14 16 2 4 6 8 10 12 14 16
