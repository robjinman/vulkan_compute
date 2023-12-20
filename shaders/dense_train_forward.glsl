#version 450

layout(constant_id = 0) const uint local_size_x = 16;
layout(constant_id = 1) const uint local_size_y = 1;
layout(constant_id = 2) const uint local_size_z = 1;
layout(local_size_x_id = 0, local_size_y_id = 1, local_size_z_id = 2) in;

layout(std140, binding = 0) buffer InputsSsbo {
  vec4 inputs[];
};

layout(std140, binding = 1) buffer WSsbo {
  vec4 W[];
};

layout(std140, binding = 2) buffer BSsbo {
  vec4 B[];
};

layout(std140, binding = 3) buffer ZSsbo {
  vec4 Z[];
};

layout(std140, binding = 4) buffer ASsbo {
  vec4 A[];
};

void main() {
  const uint outputOffset = 16; // In floats
  const uint index = gl_GlobalInvocationID.x;

  const uint vec4OutputOffset = outputOffset / 4;

  const uint vec4InputIdx = index / 4;
  const uint vec4OutputIdx = vec4OutputOffset + index / 4;

  data[vec4OutputIdx][index % 4] = data[vec4InputIdx][index % 4] * 2.0;
}

