#include "gpu.hpp"
#include "types.hpp"
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

std::string loadFile(const std::string& path) {
  std::ifstream fin(path);
  std::stringstream ss;
  std::string line;
  while (std::getline(fin, line)) {
    ss << line << std::endl;
  }
  return ss.str();
}

int main() {
  GpuPtr gpu = createGpu();

  std::vector<netfloat_t> data{
    1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8,   // Inputs
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0    // Outputs
  };

  std::string shaderSource = loadFile("shaders/shader.glsl");
  ShaderHandle shader = gpu->compileShader(shaderSource);

  gpu->submitBuffer(data.data(), data.size() * sizeof(netfloat_t));

  gpu->executeShader(shader, 16);

  gpu->retrieveBuffer(data.data());

  for (size_t i = 0; i < data.size(); ++i) {
    std::cout << data[i] << " ";
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

