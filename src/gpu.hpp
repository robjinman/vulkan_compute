#pragma once

#include <string>
#include <memory>
#include <vector>
#include <array>

using ShaderHandle = uint32_t;
using BufferHandle = uint32_t;
using Size3 = const std::array<uint32_t, 3>;
using BufferBindings = std::vector<BufferHandle>;
using BufferFlags = uint32_t;

class Gpu {
  public:
    virtual ShaderHandle compileShader(const std::string& sourcePath,
      const BufferBindings& bufferBindings, const Size3& workgroupSize) = 0;
    virtual BufferHandle allocateBuffer(size_t size, BufferFlags flags) = 0;
    virtual void submitBufferData(BufferHandle buffer, const void* data) = 0;
    virtual void queueShader(ShaderHandle shaderHandle) = 0;
    virtual void retrieveBuffer(BufferHandle buffer, void* data) = 0;
    virtual void flushQueue() = 0;

    virtual ~Gpu() {}
};

using GpuPtr = std::unique_ptr<Gpu>;

GpuPtr createGpu();
