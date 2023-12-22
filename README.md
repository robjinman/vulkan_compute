Building
--------

Install development libraries

```
  sudo apt install \
    build-essential \
    libvulkan-dev \
    vulkan-validationLayers-dev
```

From the richard subdirectory, to make a release build, run

```
    cmake -B build/release -D CMAKE_BUILD_TYPE=Release
    cmake --build build/release
```

And for a debug build:

```
    cmake -B build/debug -D CMAKE_BUILD_TYPE=Debug
    cmake --build build/debug
```
