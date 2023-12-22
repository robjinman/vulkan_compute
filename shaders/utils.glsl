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