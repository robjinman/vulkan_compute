cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME VulkanTest)
set(TARGET_NAME vulkanTest)

project(${PROJECT_NAME})

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Building ${CMAKE_BUILD_TYPE} configuration...")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(VENDOR_DIR ${PROJECT_SOURCE_DIR}/vendor/build)
set(GLSLC_EXE "${VENDOR_DIR}/shaderc/bin/glslc")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)

file(GLOB CPP_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${TARGET_NAME} ${CPP_SOURCES})

target_include_directories(
  ${TARGET_NAME}
  PRIVATE
    "${PROJECT_SOURCE_DIR}/src"
)

target_link_libraries(${TARGET_NAME} vulkan)

set(COMPILER_FLAGS -Wextra -Wall)
set(DEBUG_FLAGS ${COMPILER_FLAGS} -g)
set(RELEASE_FLAGS ${COMPILER_FLAGS} -O3 -DNDEBUG)

target_compile_options(${TARGET_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_FLAGS}>")
target_compile_options(${TARGET_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_FLAGS}>")

set(SHADER_BINARY_DIR "${PROJECT_BINARY_DIR}/shaders")

file(GLOB SHADER_SOURCES "${PROJECT_SOURCE_DIR}/shaders/*.glsl")

set(SHADER_BINARIES "")
foreach(SHADER_SOURCE ${SHADER_SOURCES})
  get_filename_component(SHADER_FILENAME ${SHADER_SOURCE} NAME)
  string(REGEX REPLACE "[.]glsl$" ".spv" SHADER_BINARY_NAME ${SHADER_FILENAME})
  set(SHADER_BINARY "${SHADER_BINARY_DIR}/${SHADER_BINARY_NAME}")
  list(APPEND SHADER_BINARIES ${SHADER_BINARY})
  add_custom_command(
    OUTPUT ${SHADER_BINARY}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SHADER_BINARY_DIR}"
    COMMAND ${GLSLC_EXE} -fshader-stage=compute ${SHADER_SOURCE} -o ${SHADER_BINARY}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    MAIN_DEPENDENCY ${SHADER_SOURCE}
  )
endforeach()
add_custom_target(shaders DEPENDS ${SHADER_BINARIES})

add_dependencies(${TARGET_NAME} shaders)

