#version 450

#define FN_READ(BUF) \
  float read##BUF(uint pos) { \
    return BUF[pos / 4][pos % 4]; \
  }

#define FN_WRITE(BUF) \
  void write##BUF(uint pos, float val) { \
    BUF[pos / 4][pos % 4] = val; \
  }

layout(constant_id = 0) const uint local_size_x = 1;
layout(constant_id = 1) const uint local_size_y = 1;
layout(constant_id = 2) const uint local_size_z = 1;
layout(local_size_x_id = 0, local_size_y_id = 1, local_size_z_id = 2) in;

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
