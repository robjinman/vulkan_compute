#version 450

#include "utils.glsl"

layout(std140, binding = 0) readonly buffer ASsbo {
  vec4 A[];
};

FN_READ(A)

layout(std140, binding = 1) writeonly buffer BSsbo {
  vec4 B[];
};

FN_WRITE(B)

void main() {
  const uint index = gl_GlobalInvocationID.x;
  writeB(index, readA(index) * 2.0);
}
