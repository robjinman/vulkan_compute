Install development libraries

```
  sudo apt install \
    build-essential \
    libvulkan-dev \
    vulkan-validationLayers-dev
```

To build the app, from project root, run

```
  mkdir -p build
  cd build
  cmake -G "Unix Makefiles" ..
  make -j8
```

