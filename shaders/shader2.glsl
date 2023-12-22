#version 450

#include "utils.glsl"

layout(std140, binding = 0) readonly buffer BSsbo {
  vec4 B[];
};

FN_READ(B)

layout(std140, binding = 1) writeonly buffer ASsbo {
  vec4 A[];
};

FN_WRITE(A)

void main() {
  const uint index = gl_GlobalInvocationID.x;
  writeA(index, readB(index) * 3.0);
}
