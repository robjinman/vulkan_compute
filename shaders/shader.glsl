#version 450

#include "utils.glsl"

layout(binding = 0) uniform UniformBufferObject {
  vec2 a;
  vec2 b;
} ubo;

layout(std140, binding = 1) readonly buffer ASsbo {
  vec4 A[];
};

FN_READ(A)

layout(std140, binding = 2) writeonly buffer BSsbo {
  vec4 B[];
};

FN_WRITE(B)

void main() {
  const uint index = gl_GlobalInvocationID.x;
  writeB(index, readA(index) * 2.0 + ubo.a.x + ubo.a.y + ubo.b.x + ubo.b.y);
}
