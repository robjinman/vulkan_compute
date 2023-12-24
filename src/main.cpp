#include "gpu.hpp"
#include "types.hpp"
#include "exception.hpp"
#include <cstdlib>
#include <chrono>
#include <iostream>

template <size_t N>
void printBuffer(const std::array<netfloat_t, N>& buffer) {
  for (size_t i = 0; i < buffer.size(); ++i) {
    std::cout << buffer[i] << " ";
  }
  std::cout << std::endl;
}

struct Ubo {
  float a[2];
  float b[2];
};

void computeExpectedOutput() {
  std::array<netfloat_t, 16> A{ 1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8 };
  std::array<netfloat_t, 16> B{};

  constexpr size_t iterations = 3;
  for (size_t i = 0; i < iterations; ++i) {
    Ubo ubo{{ i + 0.f, i + 1.f }, { i + 2.f, i + 3.f }};

    for (size_t j = 0; j < A.size(); ++j) {
      B[j] = A[j] * 2.f + ubo.a[0] + ubo.a[1] + ubo.b[0] + ubo.b[1];
    }

    for (size_t j = 0; j < A.size(); ++j) {
      A[j] = B[j] * 3.f;
    }
  }

  printBuffer(A);
  printBuffer(B);
}

int main() {
  GpuPtr gpu = createGpu();

  std::array<netfloat_t, 16> bufferAData{
    1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8
  };

  std::array<netfloat_t, 16> bufferBData{};

  GpuBuffer ubo = gpu->allocateBuffer(sizeof(Ubo),
    GpuBufferFlags::frequentHostAccess | GpuBufferFlags::shaderReadonly);

  ASSERT_MSG(ubo.data != nullptr, "Expected ubo to be memory mapped");

  GpuBuffer bufferA = gpu->allocateBuffer(bufferAData.size() * sizeof(netfloat_t),
    GpuBufferFlags::large | GpuBufferFlags::hostReadAccess | GpuBufferFlags::hostWriteAccess);

  GpuBuffer bufferB = gpu->allocateBuffer(bufferBData.size() * sizeof(netfloat_t),
    GpuBufferFlags::large | GpuBufferFlags::hostReadAccess);

  uint32_t gpuThreads = static_cast<uint32_t>(bufferAData.size());

  ShaderHandle shader1 = gpu->compileShader("shaders/shader.glsl",
    { ubo.handle, bufferA.handle, bufferB.handle }, { gpuThreads, 1, 1 });

  ShaderHandle shader2 = gpu->compileShader("shaders/shader2.glsl",
    { bufferB.handle, bufferA.handle }, { gpuThreads, 1, 1 });

  auto startTime = std::chrono::high_resolution_clock::now();

  gpu->submitBufferData(bufferA.handle, bufferAData.data());

  constexpr size_t iterations = 3;
  for (size_t i = 0; i < iterations; ++i) {
    Ubo& uboData = *reinterpret_cast<Ubo*>(ubo.data);
    uboData = Ubo{{ i + 0.f, i + 1.f }, { i + 2.f, i + 3.f }};

    gpu->queueShader(shader1);
    gpu->queueShader(shader2);

    gpu->flushQueue();
  }

  gpu->retrieveBuffer(bufferA.handle, bufferAData.data());
  gpu->retrieveBuffer(bufferB.handle, bufferBData.data());

  auto endTime = std::chrono::high_resolution_clock::now();
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

  printBuffer(bufferAData);
  printBuffer(bufferBData);

  std::cout << "Time elapsed: " << time << " microseconds" << std::endl;

  computeExpectedOutput();

  return EXIT_SUCCESS;
}
