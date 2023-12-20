# 🐺++ volk-cpp [![Build Status](https://github.com/rokuz/volk-cpp/workflows/build/badge.svg)](https://github.com/rokuz/volk-cpp/actions) 

C++20 port of [volk library](https://github.com/zeux/volk).

The usage is essentially the same as in the original volk library, however there is one key difference (except C++20):
**No global variables and functions**.

```cpp
  Volk volk;
  if (volk.getStatus() != VK_SUCCESS) {
    printf("volk-cpp initialization failed!\n");
    return -1;
  }

  auto version = volk.getInstanceVersion();
  printf("Vulkan version %d.%d.%d initialized.\n",
          VK_VERSION_MAJOR(version),
          VK_VERSION_MINOR(version),
          VK_VERSION_PATCH(version));

  uint32_t cnt = 0;
  volk.vkEnumerateInstanceLayerProperties(&cnt, nullptr);
```

## License

This library is available to anybody free of charge, under the terms of MIT License (see LICENSE.md).

Original [volk license](https://github.com/zeux/volk/blob/master/LICENSE.md).
