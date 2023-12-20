#include <volk.hpp>

#include <cstdio>
#include <cstdlib>

int main() {
  // This won't compile if the appropriate Vulkan platform define isn't set.
  auto ptr =
#if defined(_WIN32)
  &Volk::vkCreateWin32SurfaceKHR;
#elif defined(__linux__) || defined(__unix__)
  &Volk::vkCreateXlibSurfaceKHR;
#elif defined(__APPLE__)
  &Volk::vkCreateMacOSSurfaceMVK;
#else
  nullptr;
  #error Platform not recogized for testing.
#endif

  // Try to initialize volk. This might not work on CI builds, but the
  // above should have compiled at least.
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

  return 0;
}
