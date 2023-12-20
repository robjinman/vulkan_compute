#version 450

layout(constant_id = 0) const uint local_size_x = 1;
layout(constant_id = 1) const uint local_size_y = 1;
layout(constant_id = 2) const uint local_size_z = 1;
layout(local_size_x_id = 0, local_size_y_id = 1, local_size_z_id = 2) in;

layout(std140, binding = 0) buffer DataSsboIn {
  vec4 data[];
};

float read(uint pos) {
  return data[pos / 4][pos % 4];
}

void write(uint pos, float val) {
  data[pos / 4][pos % 4] = val;
}

void main() {
  const uint inputOffset = 16; // In floats
  const uint outputOffset = 0; // In floats
  const uint index = gl_GlobalInvocationID.x;

  write(outputOffset + index, read(inputOffset + index) * 2.0);
}
