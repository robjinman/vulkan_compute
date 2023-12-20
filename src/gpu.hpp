#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>

using ShaderHandle = size_t;
using Size3 = const std::array<uint32_t, 3>;

class Gpu {
  public:
    virtual ShaderHandle compileShader(const std::string& source, const Size3& workgroupSize) = 0;
    virtual void submitBuffer(const void* buffer, size_t bufferSize) = 0; // TODO: queueSubmitBuffer
    virtual void queueShader(size_t shaderIndex) = 0;
    virtual void retrieveBuffer(void* data) = 0; // TODO: queueRetrieveBuffer
    virtual void flushQueue() = 0;

    virtual ~Gpu() {}
};

using GpuPtr = std::unique_ptr<Gpu>;

GpuPtr createGpu();
