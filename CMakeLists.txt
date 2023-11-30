cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME Compute)
set(TARGET_NAME compute)

project(${PROJECT_NAME})

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Building ${CMAKE_BUILD_TYPE} configuration...")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/dependencies/${CMAKE_BUILD_TYPE})

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)

FetchContent_MakeAvailable(googletest)

set(ABSL_ENABLE_INSTALL ON)
set(ABSL_PROPAGATE_CXX_STD ON)

FetchContent_Declare(
  absl
  URL https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.1.tar.gz
)

FetchContent_MakeAvailable(absl)

FetchContent_Declare(
  re2
  URL https://github.com/google/re2/archive/refs/tags/2023-11-01.tar.gz
)

FetchContent_MakeAvailable(re2)

FetchContent_Declare(
  effcee
  URL https://github.com/google/effcee/archive/refs/tags/v2019.1.tar.gz
)

FetchContent_MakeAvailable(effcee)

FetchContent_Declare(
  spirv-headers
  URL https://github.com/KhronosGroup/SPIRV-Headers/archive/refs/tags/vulkan-sdk-1.3.268.0.tar.gz
)

FetchContent_MakeAvailable(spirv-headers)

FetchContent_Declare(
  spirv-tools
  URL https://github.com/KhronosGroup/SPIRV-Tools/archive/refs/tags/vulkan-sdk-1.3.268.0.tar.gz
)

FetchContent_MakeAvailable(spirv-tools)

FetchContent_Declare(
  glslang
  URL https://github.com/KhronosGroup/glslang/archive/refs/tags/13.1.1.tar.gz
)

FetchContent_MakeAvailable(glslang)

FetchContent_Declare(
  shaderc
  URL https://github.com/google/shaderc/archive/refs/tags/v2023.7.tar.gz
)

FetchContent_MakeAvailable(shaderc)

file(GLOB CPP_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${TARGET_NAME} ${CPP_SOURCES})

target_include_directories(
  ${TARGET_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(${TARGET_NAME} vulkan shaderc)

set(COMPILER_FLAGS -Wextra -Wall)
set(DEBUG_FLAGS ${COMPILER_FLAGS} -g)
set(RELEASE_FLAGS ${COMPILER_FLAGS} -O3 -DNDEBUG)

target_compile_options(${TARGET_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>")
target_compile_options(${TARGET_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>")

add_custom_target(
  link_shaders ALL
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/shaders ${PROJECT_BINARY_DIR}/shaders
)
