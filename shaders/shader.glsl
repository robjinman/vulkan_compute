#version 450

layout (local_size_x = 16) in;

layout(std140, binding = 0) buffer DataSsboIn {
  vec4 data[];
};

void main() {
  const uint outputOffset = 16; // In floats
  const uint index = gl_GlobalInvocationID.x;

  const uint vec4OutputOffset = outputOffset / 4;

  const uint vec4InputIdx = index / 4;
  const uint vec4OutputIdx = vec4OutputOffset + index / 4;

  data[vec4OutputIdx][index % 4] = data[vec4InputIdx][index % 4] * 2.0;
}

