/* This file is part of volk-cpp library; see volk.hpp for version/license details */
/* clang-format off */
#include "volk.hpp"

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #include <windows.h>
#else
  #include <dlfcn.h>
#endif

Volk::Volk(PFN_vkGetInstanceProcAddr handler /* = nullptr */) noexcept {
  vkGetInstanceProcAddr = handler;
  if (!vkGetInstanceProcAddr) {
#if defined(_WIN32)
    auto m = LoadLibraryA("vulkan-1.dll");
    if (!m) {
      status_ = VK_ERROR_INITIALIZATION_FAILED;
      return;
    }
    
    // note: function pointer is cast through void function pointer to silence cast-function-type warning on gcc8
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(void(*)(void))GetProcAddress(m, "vkGetInstanceProcAddr");
#elif defined(__APPLE__)
    auto m = dlopen("libvulkan.dylib", RTLD_NOW | RTLD_LOCAL);
    if (!m) {
      m = dlopen("libvulkan.1.dylib", RTLD_NOW | RTLD_LOCAL);
    }
    if (!m) {
      m = dlopen("libMoltenVK.dylib", RTLD_NOW | RTLD_LOCAL);
    }
    if (!m) {
      status_ = VK_ERROR_INITIALIZATION_FAILED;
      return;
    }
    
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(m, "vkGetInstanceProcAddr");
#else
    auto m = dlopen("libvulkan.so.1", RTLD_NOW | RTLD_LOCAL);
    if (!m) {
      m = dlopen("libvulkan.so", RTLD_NOW | RTLD_LOCAL);
    }
    if (!m) {
      status_ = VK_ERROR_INITIALIZATION_FAILED;
      return;
    }
    
    vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)dlsym(m, "vkGetInstanceProcAddr");
#endif
    loadedModule_ = m;
  }

  genLoadLoader(nullptr, &Volk::vkGetInstanceProcAddrStub);
}

Volk::~Volk() noexcept {
  if (loadedModule_) {
#if defined(_WIN32)
    FreeLibrary((HMODULE)loadedModule_);
#else
    dlclose(loadedModule_);
#endif
  }

  vkGetInstanceProcAddr = nullptr;
  genLoadLoader(nullptr, &Volk::nullProcAddrStub);
  genLoadInstance(nullptr, &Volk::nullProcAddrStub);
  genLoadDevice(nullptr, &Volk::nullProcAddrStub);

  loadedModule_ = nullptr;
  loadedInstance_ = VK_NULL_HANDLE;
  loadedDevice_ = VK_NULL_HANDLE;
}

uint32_t Volk::getVolkHeaderVersion() noexcept {
  return VOLK_CPP_HEADER_VERSION;
}

VkResult Volk::getStatus() noexcept {
  return status_;
}

uint32_t Volk::getInstanceVersion() noexcept {
#if defined(VK_VERSION_1_1)
  uint32_t apiVersion = 0;
  if (vkEnumerateInstanceVersion && vkEnumerateInstanceVersion(&apiVersion) == VK_SUCCESS) {
    return apiVersion;
  }
#endif

  if (vkCreateInstance) {
    return VK_API_VERSION_1_0;
  }

  return 0;
}

void Volk::loadInstance(VkInstance instance) noexcept {
  loadedInstance_ = instance;
  genLoadInstance(instance, &Volk::vkGetInstanceProcAddrStub);
  genLoadDevice(instance, &Volk::vkGetInstanceProcAddrStub);
}

void Volk::loadInstanceOnly(VkInstance instance) noexcept {
  loadedInstance_ = instance;
  genLoadInstance(instance, &Volk::vkGetInstanceProcAddrStub);
}

VkInstance Volk::getLoadedInstance() noexcept {
  return loadedInstance_;
}

void Volk::loadDevice(VkDevice device) noexcept {
  loadedDevice_ = device;
  genLoadDevice(device, &Volk::vkGetDeviceProcAddrStub);
}

VkDevice Volk::getLoadedDevice() noexcept {
  return loadedDevice_;
}

PFN_vkVoidFunction Volk::vkGetInstanceProcAddrStub(void* context, char const* name) noexcept {
  return vkGetInstanceProcAddr(static_cast<VkInstance>(context), name);
}

PFN_vkVoidFunction Volk::vkGetDeviceProcAddrStub(void* context, char const* name) noexcept {
  return vkGetDeviceProcAddr(static_cast<VkDevice>(context), name);
}

PFN_vkVoidFunction Volk::nullProcAddrStub(void* context, char const* name) noexcept {
  (void)context;
  (void)name;
  return nullptr;
}

void Volk::genLoadLoader(void* context, PFN_vkVoidFunction (Volk::*load)(void*, char const*)) noexcept {
  /* VOLK_CPP_GENERATE_LOAD_LOADER */
#if defined(VK_BASE_VERSION_1_0)
  vkCreateInstance = (PFN_vkCreateInstance)(this->*(load))(context, "vkCreateInstance");
  vkEnumerateInstanceExtensionProperties = (PFN_vkEnumerateInstanceExtensionProperties)(this->*(load))(context, "vkEnumerateInstanceExtensionProperties");
  vkEnumerateInstanceLayerProperties = (PFN_vkEnumerateInstanceLayerProperties)(this->*(load))(context, "vkEnumerateInstanceLayerProperties");
#endif /* defined(VK_BASE_VERSION_1_0) */
#if defined(VK_BASE_VERSION_1_1)
  vkEnumerateInstanceVersion = (PFN_vkEnumerateInstanceVersion)(this->*(load))(context, "vkEnumerateInstanceVersion");
#endif /* defined(VK_BASE_VERSION_1_1) */
  /* VOLK_CPP_GENERATE_LOAD_LOADER */
}

void Volk::genLoadInstance(void* context, PFN_vkVoidFunction (Volk::*load)(void*, char const*)) noexcept {
  /* VOLK_CPP_GENERATE_LOAD_INSTANCE */
#if defined(VK_BASE_VERSION_1_0)
  vkCreateDevice = (PFN_vkCreateDevice)(this->*(load))(context, "vkCreateDevice");
  vkDestroyInstance = (PFN_vkDestroyInstance)(this->*(load))(context, "vkDestroyInstance");
  vkEnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)(this->*(load))(context, "vkEnumerateDeviceExtensionProperties");
  vkEnumerateDeviceLayerProperties = (PFN_vkEnumerateDeviceLayerProperties)(this->*(load))(context, "vkEnumerateDeviceLayerProperties");
  vkEnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)(this->*(load))(context, "vkEnumeratePhysicalDevices");
  vkGetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)(this->*(load))(context, "vkGetDeviceProcAddr");
  vkGetPhysicalDeviceFeatures = (PFN_vkGetPhysicalDeviceFeatures)(this->*(load))(context, "vkGetPhysicalDeviceFeatures");
  vkGetPhysicalDeviceFormatProperties = (PFN_vkGetPhysicalDeviceFormatProperties)(this->*(load))(context, "vkGetPhysicalDeviceFormatProperties");
  vkGetPhysicalDeviceImageFormatProperties = (PFN_vkGetPhysicalDeviceImageFormatProperties)(this->*(load))(context, "vkGetPhysicalDeviceImageFormatProperties");
  vkGetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)(this->*(load))(context, "vkGetPhysicalDeviceMemoryProperties");
  vkGetPhysicalDeviceProperties = (PFN_vkGetPhysicalDeviceProperties)(this->*(load))(context, "vkGetPhysicalDeviceProperties");
  vkGetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)(this->*(load))(context, "vkGetPhysicalDeviceQueueFamilyProperties");
  vkGetPhysicalDeviceSparseImageFormatProperties = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties)(this->*(load))(context, "vkGetPhysicalDeviceSparseImageFormatProperties");
#endif /* defined(VK_BASE_VERSION_1_0) */
#if defined(VK_BASE_VERSION_1_1)
  vkEnumeratePhysicalDeviceGroups = (PFN_vkEnumeratePhysicalDeviceGroups)(this->*(load))(context, "vkEnumeratePhysicalDeviceGroups");
  vkGetPhysicalDeviceExternalBufferProperties = (PFN_vkGetPhysicalDeviceExternalBufferProperties)(this->*(load))(context, "vkGetPhysicalDeviceExternalBufferProperties");
  vkGetPhysicalDeviceExternalFenceProperties = (PFN_vkGetPhysicalDeviceExternalFenceProperties)(this->*(load))(context, "vkGetPhysicalDeviceExternalFenceProperties");
  vkGetPhysicalDeviceExternalSemaphoreProperties = (PFN_vkGetPhysicalDeviceExternalSemaphoreProperties)(this->*(load))(context, "vkGetPhysicalDeviceExternalSemaphoreProperties");
  vkGetPhysicalDeviceFeatures2 = (PFN_vkGetPhysicalDeviceFeatures2)(this->*(load))(context, "vkGetPhysicalDeviceFeatures2");
  vkGetPhysicalDeviceFormatProperties2 = (PFN_vkGetPhysicalDeviceFormatProperties2)(this->*(load))(context, "vkGetPhysicalDeviceFormatProperties2");
  vkGetPhysicalDeviceImageFormatProperties2 = (PFN_vkGetPhysicalDeviceImageFormatProperties2)(this->*(load))(context, "vkGetPhysicalDeviceImageFormatProperties2");
  vkGetPhysicalDeviceMemoryProperties2 = (PFN_vkGetPhysicalDeviceMemoryProperties2)(this->*(load))(context, "vkGetPhysicalDeviceMemoryProperties2");
  vkGetPhysicalDeviceProperties2 = (PFN_vkGetPhysicalDeviceProperties2)(this->*(load))(context, "vkGetPhysicalDeviceProperties2");
  vkGetPhysicalDeviceQueueFamilyProperties2 = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2)(this->*(load))(context, "vkGetPhysicalDeviceQueueFamilyProperties2");
  vkGetPhysicalDeviceSparseImageFormatProperties2 = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2)(this->*(load))(context, "vkGetPhysicalDeviceSparseImageFormatProperties2");
#endif /* defined(VK_BASE_VERSION_1_1) */
#if defined(VK_BASE_VERSION_1_3)
  vkGetPhysicalDeviceToolProperties = (PFN_vkGetPhysicalDeviceToolProperties)(this->*(load))(context, "vkGetPhysicalDeviceToolProperties");
#endif /* defined(VK_BASE_VERSION_1_3) */
#if defined(VK_ARM_data_graph)
  vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM)(this->*(load))(context, "vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM");
  vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM = (PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM)(this->*(load))(context, "vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM");
#endif /* defined(VK_ARM_data_graph) */
#if defined(VK_ARM_performance_counters_by_region)
  vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM)(this->*(load))(context, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM");
#endif /* defined(VK_ARM_performance_counters_by_region) */
#if defined(VK_ARM_tensors)
  vkGetPhysicalDeviceExternalTensorPropertiesARM = (PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM)(this->*(load))(context, "vkGetPhysicalDeviceExternalTensorPropertiesARM");
#endif /* defined(VK_ARM_tensors) */
#if defined(VK_EXT_acquire_drm_display)
  vkAcquireDrmDisplayEXT = (PFN_vkAcquireDrmDisplayEXT)(this->*(load))(context, "vkAcquireDrmDisplayEXT");
  vkGetDrmDisplayEXT = (PFN_vkGetDrmDisplayEXT)(this->*(load))(context, "vkGetDrmDisplayEXT");
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
  vkAcquireXlibDisplayEXT = (PFN_vkAcquireXlibDisplayEXT)(this->*(load))(context, "vkAcquireXlibDisplayEXT");
  vkGetRandROutputDisplayEXT = (PFN_vkGetRandROutputDisplayEXT)(this->*(load))(context, "vkGetRandROutputDisplayEXT");
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_calibrated_timestamps)
  vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT)(this->*(load))(context, "vkGetPhysicalDeviceCalibrateableTimeDomainsEXT");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_debug_report)
  vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)(this->*(load))(context, "vkCreateDebugReportCallbackEXT");
  vkDebugReportMessageEXT = (PFN_vkDebugReportMessageEXT)(this->*(load))(context, "vkDebugReportMessageEXT");
  vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)(this->*(load))(context, "vkDestroyDebugReportCallbackEXT");
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
  vkCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)(this->*(load))(context, "vkCmdBeginDebugUtilsLabelEXT");
  vkCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)(this->*(load))(context, "vkCmdEndDebugUtilsLabelEXT");
  vkCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)(this->*(load))(context, "vkCmdInsertDebugUtilsLabelEXT");
  vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)(this->*(load))(context, "vkCreateDebugUtilsMessengerEXT");
  vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)(this->*(load))(context, "vkDestroyDebugUtilsMessengerEXT");
  vkQueueBeginDebugUtilsLabelEXT = (PFN_vkQueueBeginDebugUtilsLabelEXT)(this->*(load))(context, "vkQueueBeginDebugUtilsLabelEXT");
  vkQueueEndDebugUtilsLabelEXT = (PFN_vkQueueEndDebugUtilsLabelEXT)(this->*(load))(context, "vkQueueEndDebugUtilsLabelEXT");
  vkQueueInsertDebugUtilsLabelEXT = (PFN_vkQueueInsertDebugUtilsLabelEXT)(this->*(load))(context, "vkQueueInsertDebugUtilsLabelEXT");
  vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)(this->*(load))(context, "vkSetDebugUtilsObjectNameEXT");
  vkSetDebugUtilsObjectTagEXT = (PFN_vkSetDebugUtilsObjectTagEXT)(this->*(load))(context, "vkSetDebugUtilsObjectTagEXT");
  vkSubmitDebugUtilsMessageEXT = (PFN_vkSubmitDebugUtilsMessageEXT)(this->*(load))(context, "vkSubmitDebugUtilsMessageEXT");
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
  vkReleaseDisplayEXT = (PFN_vkReleaseDisplayEXT)(this->*(load))(context, "vkReleaseDisplayEXT");
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
  vkCreateDirectFBSurfaceEXT = (PFN_vkCreateDirectFBSurfaceEXT)(this->*(load))(context, "vkCreateDirectFBSurfaceEXT");
  vkGetPhysicalDeviceDirectFBPresentationSupportEXT = (PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT)(this->*(load))(context, "vkGetPhysicalDeviceDirectFBPresentationSupportEXT");
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_display_surface_counter)
  vkGetPhysicalDeviceSurfaceCapabilities2EXT = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT)(this->*(load))(context, "vkGetPhysicalDeviceSurfaceCapabilities2EXT");
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_full_screen_exclusive)
  vkGetPhysicalDeviceSurfacePresentModes2EXT = (PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT)(this->*(load))(context, "vkGetPhysicalDeviceSurfacePresentModes2EXT");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_headless_surface)
  vkCreateHeadlessSurfaceEXT = (PFN_vkCreateHeadlessSurfaceEXT)(this->*(load))(context, "vkCreateHeadlessSurfaceEXT");
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_metal_surface)
  vkCreateMetalSurfaceEXT = (PFN_vkCreateMetalSurfaceEXT)(this->*(load))(context, "vkCreateMetalSurfaceEXT");
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_sample_locations)
  vkGetPhysicalDeviceMultisamplePropertiesEXT = (PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT)(this->*(load))(context, "vkGetPhysicalDeviceMultisamplePropertiesEXT");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_tooling_info)
  vkGetPhysicalDeviceToolPropertiesEXT = (PFN_vkGetPhysicalDeviceToolPropertiesEXT)(this->*(load))(context, "vkGetPhysicalDeviceToolPropertiesEXT");
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_FUCHSIA_imagepipe_surface)
  vkCreateImagePipeSurfaceFUCHSIA = (PFN_vkCreateImagePipeSurfaceFUCHSIA)(this->*(load))(context, "vkCreateImagePipeSurfaceFUCHSIA");
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
  vkCreateStreamDescriptorSurfaceGGP = (PFN_vkCreateStreamDescriptorSurfaceGGP)(this->*(load))(context, "vkCreateStreamDescriptorSurfaceGGP");
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_KHR_android_surface)
  vkCreateAndroidSurfaceKHR = (PFN_vkCreateAndroidSurfaceKHR)(this->*(load))(context, "vkCreateAndroidSurfaceKHR");
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_calibrated_timestamps)
  vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = (PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR)(this->*(load))(context, "vkGetPhysicalDeviceCalibrateableTimeDomainsKHR");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_cooperative_matrix)
  vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR");
#endif /* defined(VK_KHR_cooperative_matrix) */
#if defined(VK_KHR_device_group_creation)
  vkEnumeratePhysicalDeviceGroupsKHR = (PFN_vkEnumeratePhysicalDeviceGroupsKHR)(this->*(load))(context, "vkEnumeratePhysicalDeviceGroupsKHR");
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
  vkCreateDisplayModeKHR = (PFN_vkCreateDisplayModeKHR)(this->*(load))(context, "vkCreateDisplayModeKHR");
  vkCreateDisplayPlaneSurfaceKHR = (PFN_vkCreateDisplayPlaneSurfaceKHR)(this->*(load))(context, "vkCreateDisplayPlaneSurfaceKHR");
  vkGetDisplayModePropertiesKHR = (PFN_vkGetDisplayModePropertiesKHR)(this->*(load))(context, "vkGetDisplayModePropertiesKHR");
  vkGetDisplayPlaneCapabilitiesKHR = (PFN_vkGetDisplayPlaneCapabilitiesKHR)(this->*(load))(context, "vkGetDisplayPlaneCapabilitiesKHR");
  vkGetDisplayPlaneSupportedDisplaysKHR = (PFN_vkGetDisplayPlaneSupportedDisplaysKHR)(this->*(load))(context, "vkGetDisplayPlaneSupportedDisplaysKHR");
  vkGetPhysicalDeviceDisplayPlanePropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR");
  vkGetPhysicalDeviceDisplayPropertiesKHR = (PFN_vkGetPhysicalDeviceDisplayPropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceDisplayPropertiesKHR");
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_external_fence_capabilities)
  vkGetPhysicalDeviceExternalFencePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceExternalFencePropertiesKHR");
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_memory_capabilities)
  vkGetPhysicalDeviceExternalBufferPropertiesKHR = (PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceExternalBufferPropertiesKHR");
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_semaphore_capabilities)
  vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = (PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceExternalSemaphorePropertiesKHR");
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_fragment_shading_rate)
  vkGetPhysicalDeviceFragmentShadingRatesKHR = (PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR)(this->*(load))(context, "vkGetPhysicalDeviceFragmentShadingRatesKHR");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
  vkGetDisplayModeProperties2KHR = (PFN_vkGetDisplayModeProperties2KHR)(this->*(load))(context, "vkGetDisplayModeProperties2KHR");
  vkGetDisplayPlaneCapabilities2KHR = (PFN_vkGetDisplayPlaneCapabilities2KHR)(this->*(load))(context, "vkGetDisplayPlaneCapabilities2KHR");
  vkGetPhysicalDeviceDisplayPlaneProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceDisplayPlaneProperties2KHR");
  vkGetPhysicalDeviceDisplayProperties2KHR = (PFN_vkGetPhysicalDeviceDisplayProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceDisplayProperties2KHR");
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_physical_device_properties2)
  vkGetPhysicalDeviceFeatures2KHR = (PFN_vkGetPhysicalDeviceFeatures2KHR)(this->*(load))(context, "vkGetPhysicalDeviceFeatures2KHR");
  vkGetPhysicalDeviceFormatProperties2KHR = (PFN_vkGetPhysicalDeviceFormatProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceFormatProperties2KHR");
  vkGetPhysicalDeviceImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceImageFormatProperties2KHR");
  vkGetPhysicalDeviceMemoryProperties2KHR = (PFN_vkGetPhysicalDeviceMemoryProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceMemoryProperties2KHR");
  vkGetPhysicalDeviceProperties2KHR = (PFN_vkGetPhysicalDeviceProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceProperties2KHR");
  vkGetPhysicalDeviceQueueFamilyProperties2KHR = (PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceQueueFamilyProperties2KHR");
  vkGetPhysicalDeviceSparseImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR)(this->*(load))(context, "vkGetPhysicalDeviceSparseImageFormatProperties2KHR");
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
  vkGetPhysicalDeviceSurfaceCapabilities2KHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR)(this->*(load))(context, "vkGetPhysicalDeviceSurfaceCapabilities2KHR");
  vkGetPhysicalDeviceSurfaceFormats2KHR = (PFN_vkGetPhysicalDeviceSurfaceFormats2KHR)(this->*(load))(context, "vkGetPhysicalDeviceSurfaceFormats2KHR");
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_performance_query)
  vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = (PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR)(this->*(load))(context, "vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR");
  vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = (PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR)(this->*(load))(context, "vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_surface)
  vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)(this->*(load))(context, "vkDestroySurfaceKHR");
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR = (PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR");
  vkGetPhysicalDeviceSurfaceFormatsKHR = (PFN_vkGetPhysicalDeviceSurfaceFormatsKHR)(this->*(load))(context, "vkGetPhysicalDeviceSurfaceFormatsKHR");
  vkGetPhysicalDeviceSurfacePresentModesKHR = (PFN_vkGetPhysicalDeviceSurfacePresentModesKHR)(this->*(load))(context, "vkGetPhysicalDeviceSurfacePresentModesKHR");
  vkGetPhysicalDeviceSurfaceSupportKHR = (PFN_vkGetPhysicalDeviceSurfaceSupportKHR)(this->*(load))(context, "vkGetPhysicalDeviceSurfaceSupportKHR");
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_video_encode_queue)
  vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
  vkGetPhysicalDeviceVideoCapabilitiesKHR = (PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceVideoCapabilitiesKHR");
  vkGetPhysicalDeviceVideoFormatPropertiesKHR = (PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR)(this->*(load))(context, "vkGetPhysicalDeviceVideoFormatPropertiesKHR");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
  vkCreateWaylandSurfaceKHR = (PFN_vkCreateWaylandSurfaceKHR)(this->*(load))(context, "vkCreateWaylandSurfaceKHR");
  vkGetPhysicalDeviceWaylandPresentationSupportKHR = (PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)(this->*(load))(context, "vkGetPhysicalDeviceWaylandPresentationSupportKHR");
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
  vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)(this->*(load))(context, "vkCreateWin32SurfaceKHR");
  vkGetPhysicalDeviceWin32PresentationSupportKHR = (PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)(this->*(load))(context, "vkGetPhysicalDeviceWin32PresentationSupportKHR");
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
  vkCreateXcbSurfaceKHR = (PFN_vkCreateXcbSurfaceKHR)(this->*(load))(context, "vkCreateXcbSurfaceKHR");
  vkGetPhysicalDeviceXcbPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)(this->*(load))(context, "vkGetPhysicalDeviceXcbPresentationSupportKHR");
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
  vkCreateXlibSurfaceKHR = (PFN_vkCreateXlibSurfaceKHR)(this->*(load))(context, "vkCreateXlibSurfaceKHR");
  vkGetPhysicalDeviceXlibPresentationSupportKHR = (PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)(this->*(load))(context, "vkGetPhysicalDeviceXlibPresentationSupportKHR");
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
  vkCreateIOSSurfaceMVK = (PFN_vkCreateIOSSurfaceMVK)(this->*(load))(context, "vkCreateIOSSurfaceMVK");
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
  vkCreateMacOSSurfaceMVK = (PFN_vkCreateMacOSSurfaceMVK)(this->*(load))(context, "vkCreateMacOSSurfaceMVK");
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
  vkCreateViSurfaceNN = (PFN_vkCreateViSurfaceNN)(this->*(load))(context, "vkCreateViSurfaceNN");
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NV_acquire_winrt_display)
  vkAcquireWinrtDisplayNV = (PFN_vkAcquireWinrtDisplayNV)(this->*(load))(context, "vkAcquireWinrtDisplayNV");
  vkGetWinrtDisplayNV = (PFN_vkGetWinrtDisplayNV)(this->*(load))(context, "vkGetWinrtDisplayNV");
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_cooperative_matrix)
  vkGetPhysicalDeviceCooperativeMatrixPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)(this->*(load))(context, "vkGetPhysicalDeviceCooperativeMatrixPropertiesNV");
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_cooperative_matrix2)
  vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV)(this->*(load))(context, "vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV");
#endif /* defined(VK_NV_cooperative_matrix2) */
#if defined(VK_NV_cooperative_vector)
  vkGetPhysicalDeviceCooperativeVectorPropertiesNV = (PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV)(this->*(load))(context, "vkGetPhysicalDeviceCooperativeVectorPropertiesNV");
#endif /* defined(VK_NV_cooperative_vector) */
#if defined(VK_NV_coverage_reduction_mode)
  vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = (PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV)(this->*(load))(context, "vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV");
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_external_memory_capabilities)
  vkGetPhysicalDeviceExternalImageFormatPropertiesNV = (PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV)(this->*(load))(context, "vkGetPhysicalDeviceExternalImageFormatPropertiesNV");
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_optical_flow)
  vkGetPhysicalDeviceOpticalFlowImageFormatsNV = (PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV)(this->*(load))(context, "vkGetPhysicalDeviceOpticalFlowImageFormatsNV");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_OHOS_surface)
  vkCreateSurfaceOHOS = (PFN_vkCreateSurfaceOHOS)(this->*(load))(context, "vkCreateSurfaceOHOS");
#endif /* defined(VK_OHOS_surface) */
#if defined(VK_QNX_screen_surface)
  vkCreateScreenSurfaceQNX = (PFN_vkCreateScreenSurfaceQNX)(this->*(load))(context, "vkCreateScreenSurfaceQNX");
  vkGetPhysicalDeviceScreenPresentationSupportQNX = (PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX)(this->*(load))(context, "vkGetPhysicalDeviceScreenPresentationSupportQNX");
#endif /* defined(VK_QNX_screen_surface) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
  vkGetPhysicalDevicePresentRectanglesKHR = (PFN_vkGetPhysicalDevicePresentRectanglesKHR)(this->*(load))(context, "vkGetPhysicalDevicePresentRectanglesKHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
  /* VOLK_CPP_GENERATE_LOAD_INSTANCE */
}

void Volk::genLoadDevice(void* context, PFN_vkVoidFunction (Volk::*load)(void*, char const*)) noexcept {
  /* VOLK_CPP_GENERATE_LOAD_DEVICE */
#if defined(VK_BASE_VERSION_1_0)
  vkAllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)(this->*(load))(context, "vkAllocateCommandBuffers");
  vkAllocateMemory = (PFN_vkAllocateMemory)(this->*(load))(context, "vkAllocateMemory");
  vkBeginCommandBuffer = (PFN_vkBeginCommandBuffer)(this->*(load))(context, "vkBeginCommandBuffer");
  vkBindBufferMemory = (PFN_vkBindBufferMemory)(this->*(load))(context, "vkBindBufferMemory");
  vkBindImageMemory = (PFN_vkBindImageMemory)(this->*(load))(context, "vkBindImageMemory");
  vkCmdBeginQuery = (PFN_vkCmdBeginQuery)(this->*(load))(context, "vkCmdBeginQuery");
  vkCmdCopyBuffer = (PFN_vkCmdCopyBuffer)(this->*(load))(context, "vkCmdCopyBuffer");
  vkCmdCopyBufferToImage = (PFN_vkCmdCopyBufferToImage)(this->*(load))(context, "vkCmdCopyBufferToImage");
  vkCmdCopyImage = (PFN_vkCmdCopyImage)(this->*(load))(context, "vkCmdCopyImage");
  vkCmdCopyImageToBuffer = (PFN_vkCmdCopyImageToBuffer)(this->*(load))(context, "vkCmdCopyImageToBuffer");
  vkCmdCopyQueryPoolResults = (PFN_vkCmdCopyQueryPoolResults)(this->*(load))(context, "vkCmdCopyQueryPoolResults");
  vkCmdEndQuery = (PFN_vkCmdEndQuery)(this->*(load))(context, "vkCmdEndQuery");
  vkCmdExecuteCommands = (PFN_vkCmdExecuteCommands)(this->*(load))(context, "vkCmdExecuteCommands");
  vkCmdFillBuffer = (PFN_vkCmdFillBuffer)(this->*(load))(context, "vkCmdFillBuffer");
  vkCmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)(this->*(load))(context, "vkCmdPipelineBarrier");
  vkCmdResetQueryPool = (PFN_vkCmdResetQueryPool)(this->*(load))(context, "vkCmdResetQueryPool");
  vkCmdUpdateBuffer = (PFN_vkCmdUpdateBuffer)(this->*(load))(context, "vkCmdUpdateBuffer");
  vkCmdWriteTimestamp = (PFN_vkCmdWriteTimestamp)(this->*(load))(context, "vkCmdWriteTimestamp");
  vkCreateBuffer = (PFN_vkCreateBuffer)(this->*(load))(context, "vkCreateBuffer");
  vkCreateCommandPool = (PFN_vkCreateCommandPool)(this->*(load))(context, "vkCreateCommandPool");
  vkCreateFence = (PFN_vkCreateFence)(this->*(load))(context, "vkCreateFence");
  vkCreateImage = (PFN_vkCreateImage)(this->*(load))(context, "vkCreateImage");
  vkCreateImageView = (PFN_vkCreateImageView)(this->*(load))(context, "vkCreateImageView");
  vkCreateQueryPool = (PFN_vkCreateQueryPool)(this->*(load))(context, "vkCreateQueryPool");
  vkCreateSemaphore = (PFN_vkCreateSemaphore)(this->*(load))(context, "vkCreateSemaphore");
  vkDestroyBuffer = (PFN_vkDestroyBuffer)(this->*(load))(context, "vkDestroyBuffer");
  vkDestroyCommandPool = (PFN_vkDestroyCommandPool)(this->*(load))(context, "vkDestroyCommandPool");
  vkDestroyDevice = (PFN_vkDestroyDevice)(this->*(load))(context, "vkDestroyDevice");
  vkDestroyFence = (PFN_vkDestroyFence)(this->*(load))(context, "vkDestroyFence");
  vkDestroyImage = (PFN_vkDestroyImage)(this->*(load))(context, "vkDestroyImage");
  vkDestroyImageView = (PFN_vkDestroyImageView)(this->*(load))(context, "vkDestroyImageView");
  vkDestroyQueryPool = (PFN_vkDestroyQueryPool)(this->*(load))(context, "vkDestroyQueryPool");
  vkDestroySemaphore = (PFN_vkDestroySemaphore)(this->*(load))(context, "vkDestroySemaphore");
  vkDeviceWaitIdle = (PFN_vkDeviceWaitIdle)(this->*(load))(context, "vkDeviceWaitIdle");
  vkEndCommandBuffer = (PFN_vkEndCommandBuffer)(this->*(load))(context, "vkEndCommandBuffer");
  vkFlushMappedMemoryRanges = (PFN_vkFlushMappedMemoryRanges)(this->*(load))(context, "vkFlushMappedMemoryRanges");
  vkFreeCommandBuffers = (PFN_vkFreeCommandBuffers)(this->*(load))(context, "vkFreeCommandBuffers");
  vkFreeMemory = (PFN_vkFreeMemory)(this->*(load))(context, "vkFreeMemory");
  vkGetBufferMemoryRequirements = (PFN_vkGetBufferMemoryRequirements)(this->*(load))(context, "vkGetBufferMemoryRequirements");
  vkGetDeviceMemoryCommitment = (PFN_vkGetDeviceMemoryCommitment)(this->*(load))(context, "vkGetDeviceMemoryCommitment");
  vkGetDeviceQueue = (PFN_vkGetDeviceQueue)(this->*(load))(context, "vkGetDeviceQueue");
  vkGetFenceStatus = (PFN_vkGetFenceStatus)(this->*(load))(context, "vkGetFenceStatus");
  vkGetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)(this->*(load))(context, "vkGetImageMemoryRequirements");
  vkGetImageSparseMemoryRequirements = (PFN_vkGetImageSparseMemoryRequirements)(this->*(load))(context, "vkGetImageSparseMemoryRequirements");
  vkGetImageSubresourceLayout = (PFN_vkGetImageSubresourceLayout)(this->*(load))(context, "vkGetImageSubresourceLayout");
  vkGetQueryPoolResults = (PFN_vkGetQueryPoolResults)(this->*(load))(context, "vkGetQueryPoolResults");
  vkInvalidateMappedMemoryRanges = (PFN_vkInvalidateMappedMemoryRanges)(this->*(load))(context, "vkInvalidateMappedMemoryRanges");
  vkMapMemory = (PFN_vkMapMemory)(this->*(load))(context, "vkMapMemory");
  vkQueueBindSparse = (PFN_vkQueueBindSparse)(this->*(load))(context, "vkQueueBindSparse");
  vkQueueSubmit = (PFN_vkQueueSubmit)(this->*(load))(context, "vkQueueSubmit");
  vkQueueWaitIdle = (PFN_vkQueueWaitIdle)(this->*(load))(context, "vkQueueWaitIdle");
  vkResetCommandBuffer = (PFN_vkResetCommandBuffer)(this->*(load))(context, "vkResetCommandBuffer");
  vkResetCommandPool = (PFN_vkResetCommandPool)(this->*(load))(context, "vkResetCommandPool");
  vkResetFences = (PFN_vkResetFences)(this->*(load))(context, "vkResetFences");
  vkUnmapMemory = (PFN_vkUnmapMemory)(this->*(load))(context, "vkUnmapMemory");
  vkWaitForFences = (PFN_vkWaitForFences)(this->*(load))(context, "vkWaitForFences");
#endif /* defined(VK_BASE_VERSION_1_0) */
#if defined(VK_COMPUTE_VERSION_1_0)
  vkAllocateDescriptorSets = (PFN_vkAllocateDescriptorSets)(this->*(load))(context, "vkAllocateDescriptorSets");
  vkCmdBindDescriptorSets = (PFN_vkCmdBindDescriptorSets)(this->*(load))(context, "vkCmdBindDescriptorSets");
  vkCmdBindPipeline = (PFN_vkCmdBindPipeline)(this->*(load))(context, "vkCmdBindPipeline");
  vkCmdClearColorImage = (PFN_vkCmdClearColorImage)(this->*(load))(context, "vkCmdClearColorImage");
  vkCmdDispatch = (PFN_vkCmdDispatch)(this->*(load))(context, "vkCmdDispatch");
  vkCmdDispatchIndirect = (PFN_vkCmdDispatchIndirect)(this->*(load))(context, "vkCmdDispatchIndirect");
  vkCmdPushConstants = (PFN_vkCmdPushConstants)(this->*(load))(context, "vkCmdPushConstants");
  vkCmdResetEvent = (PFN_vkCmdResetEvent)(this->*(load))(context, "vkCmdResetEvent");
  vkCmdSetEvent = (PFN_vkCmdSetEvent)(this->*(load))(context, "vkCmdSetEvent");
  vkCmdWaitEvents = (PFN_vkCmdWaitEvents)(this->*(load))(context, "vkCmdWaitEvents");
  vkCreateBufferView = (PFN_vkCreateBufferView)(this->*(load))(context, "vkCreateBufferView");
  vkCreateComputePipelines = (PFN_vkCreateComputePipelines)(this->*(load))(context, "vkCreateComputePipelines");
  vkCreateDescriptorPool = (PFN_vkCreateDescriptorPool)(this->*(load))(context, "vkCreateDescriptorPool");
  vkCreateDescriptorSetLayout = (PFN_vkCreateDescriptorSetLayout)(this->*(load))(context, "vkCreateDescriptorSetLayout");
  vkCreateEvent = (PFN_vkCreateEvent)(this->*(load))(context, "vkCreateEvent");
  vkCreatePipelineCache = (PFN_vkCreatePipelineCache)(this->*(load))(context, "vkCreatePipelineCache");
  vkCreatePipelineLayout = (PFN_vkCreatePipelineLayout)(this->*(load))(context, "vkCreatePipelineLayout");
  vkCreateSampler = (PFN_vkCreateSampler)(this->*(load))(context, "vkCreateSampler");
  vkCreateShaderModule = (PFN_vkCreateShaderModule)(this->*(load))(context, "vkCreateShaderModule");
  vkDestroyBufferView = (PFN_vkDestroyBufferView)(this->*(load))(context, "vkDestroyBufferView");
  vkDestroyDescriptorPool = (PFN_vkDestroyDescriptorPool)(this->*(load))(context, "vkDestroyDescriptorPool");
  vkDestroyDescriptorSetLayout = (PFN_vkDestroyDescriptorSetLayout)(this->*(load))(context, "vkDestroyDescriptorSetLayout");
  vkDestroyEvent = (PFN_vkDestroyEvent)(this->*(load))(context, "vkDestroyEvent");
  vkDestroyPipeline = (PFN_vkDestroyPipeline)(this->*(load))(context, "vkDestroyPipeline");
  vkDestroyPipelineCache = (PFN_vkDestroyPipelineCache)(this->*(load))(context, "vkDestroyPipelineCache");
  vkDestroyPipelineLayout = (PFN_vkDestroyPipelineLayout)(this->*(load))(context, "vkDestroyPipelineLayout");
  vkDestroySampler = (PFN_vkDestroySampler)(this->*(load))(context, "vkDestroySampler");
  vkDestroyShaderModule = (PFN_vkDestroyShaderModule)(this->*(load))(context, "vkDestroyShaderModule");
  vkFreeDescriptorSets = (PFN_vkFreeDescriptorSets)(this->*(load))(context, "vkFreeDescriptorSets");
  vkGetEventStatus = (PFN_vkGetEventStatus)(this->*(load))(context, "vkGetEventStatus");
  vkGetPipelineCacheData = (PFN_vkGetPipelineCacheData)(this->*(load))(context, "vkGetPipelineCacheData");
  vkMergePipelineCaches = (PFN_vkMergePipelineCaches)(this->*(load))(context, "vkMergePipelineCaches");
  vkResetDescriptorPool = (PFN_vkResetDescriptorPool)(this->*(load))(context, "vkResetDescriptorPool");
  vkResetEvent = (PFN_vkResetEvent)(this->*(load))(context, "vkResetEvent");
  vkSetEvent = (PFN_vkSetEvent)(this->*(load))(context, "vkSetEvent");
  vkUpdateDescriptorSets = (PFN_vkUpdateDescriptorSets)(this->*(load))(context, "vkUpdateDescriptorSets");
#endif /* defined(VK_COMPUTE_VERSION_1_0) */
#if defined(VK_GRAPHICS_VERSION_1_0)
  vkCmdBeginRenderPass = (PFN_vkCmdBeginRenderPass)(this->*(load))(context, "vkCmdBeginRenderPass");
  vkCmdBindIndexBuffer = (PFN_vkCmdBindIndexBuffer)(this->*(load))(context, "vkCmdBindIndexBuffer");
  vkCmdBindVertexBuffers = (PFN_vkCmdBindVertexBuffers)(this->*(load))(context, "vkCmdBindVertexBuffers");
  vkCmdBlitImage = (PFN_vkCmdBlitImage)(this->*(load))(context, "vkCmdBlitImage");
  vkCmdClearAttachments = (PFN_vkCmdClearAttachments)(this->*(load))(context, "vkCmdClearAttachments");
  vkCmdClearDepthStencilImage = (PFN_vkCmdClearDepthStencilImage)(this->*(load))(context, "vkCmdClearDepthStencilImage");
  vkCmdDraw = (PFN_vkCmdDraw)(this->*(load))(context, "vkCmdDraw");
  vkCmdDrawIndexed = (PFN_vkCmdDrawIndexed)(this->*(load))(context, "vkCmdDrawIndexed");
  vkCmdDrawIndexedIndirect = (PFN_vkCmdDrawIndexedIndirect)(this->*(load))(context, "vkCmdDrawIndexedIndirect");
  vkCmdDrawIndirect = (PFN_vkCmdDrawIndirect)(this->*(load))(context, "vkCmdDrawIndirect");
  vkCmdEndRenderPass = (PFN_vkCmdEndRenderPass)(this->*(load))(context, "vkCmdEndRenderPass");
  vkCmdNextSubpass = (PFN_vkCmdNextSubpass)(this->*(load))(context, "vkCmdNextSubpass");
  vkCmdResolveImage = (PFN_vkCmdResolveImage)(this->*(load))(context, "vkCmdResolveImage");
  vkCmdSetBlendConstants = (PFN_vkCmdSetBlendConstants)(this->*(load))(context, "vkCmdSetBlendConstants");
  vkCmdSetDepthBias = (PFN_vkCmdSetDepthBias)(this->*(load))(context, "vkCmdSetDepthBias");
  vkCmdSetDepthBounds = (PFN_vkCmdSetDepthBounds)(this->*(load))(context, "vkCmdSetDepthBounds");
  vkCmdSetLineWidth = (PFN_vkCmdSetLineWidth)(this->*(load))(context, "vkCmdSetLineWidth");
  vkCmdSetScissor = (PFN_vkCmdSetScissor)(this->*(load))(context, "vkCmdSetScissor");
  vkCmdSetStencilCompareMask = (PFN_vkCmdSetStencilCompareMask)(this->*(load))(context, "vkCmdSetStencilCompareMask");
  vkCmdSetStencilReference = (PFN_vkCmdSetStencilReference)(this->*(load))(context, "vkCmdSetStencilReference");
  vkCmdSetStencilWriteMask = (PFN_vkCmdSetStencilWriteMask)(this->*(load))(context, "vkCmdSetStencilWriteMask");
  vkCmdSetViewport = (PFN_vkCmdSetViewport)(this->*(load))(context, "vkCmdSetViewport");
  vkCreateFramebuffer = (PFN_vkCreateFramebuffer)(this->*(load))(context, "vkCreateFramebuffer");
  vkCreateGraphicsPipelines = (PFN_vkCreateGraphicsPipelines)(this->*(load))(context, "vkCreateGraphicsPipelines");
  vkCreateRenderPass = (PFN_vkCreateRenderPass)(this->*(load))(context, "vkCreateRenderPass");
  vkDestroyFramebuffer = (PFN_vkDestroyFramebuffer)(this->*(load))(context, "vkDestroyFramebuffer");
  vkDestroyRenderPass = (PFN_vkDestroyRenderPass)(this->*(load))(context, "vkDestroyRenderPass");
  vkGetRenderAreaGranularity = (PFN_vkGetRenderAreaGranularity)(this->*(load))(context, "vkGetRenderAreaGranularity");
#endif /* defined(VK_GRAPHICS_VERSION_1_0) */
#if defined(VK_BASE_VERSION_1_1)
  vkBindBufferMemory2 = (PFN_vkBindBufferMemory2)(this->*(load))(context, "vkBindBufferMemory2");
  vkBindImageMemory2 = (PFN_vkBindImageMemory2)(this->*(load))(context, "vkBindImageMemory2");
  vkCmdSetDeviceMask = (PFN_vkCmdSetDeviceMask)(this->*(load))(context, "vkCmdSetDeviceMask");
  vkGetBufferMemoryRequirements2 = (PFN_vkGetBufferMemoryRequirements2)(this->*(load))(context, "vkGetBufferMemoryRequirements2");
  vkGetDeviceGroupPeerMemoryFeatures = (PFN_vkGetDeviceGroupPeerMemoryFeatures)(this->*(load))(context, "vkGetDeviceGroupPeerMemoryFeatures");
  vkGetDeviceQueue2 = (PFN_vkGetDeviceQueue2)(this->*(load))(context, "vkGetDeviceQueue2");
  vkGetImageMemoryRequirements2 = (PFN_vkGetImageMemoryRequirements2)(this->*(load))(context, "vkGetImageMemoryRequirements2");
  vkGetImageSparseMemoryRequirements2 = (PFN_vkGetImageSparseMemoryRequirements2)(this->*(load))(context, "vkGetImageSparseMemoryRequirements2");
  vkTrimCommandPool = (PFN_vkTrimCommandPool)(this->*(load))(context, "vkTrimCommandPool");
#endif /* defined(VK_BASE_VERSION_1_1) */
#if defined(VK_COMPUTE_VERSION_1_1)
  vkCmdDispatchBase = (PFN_vkCmdDispatchBase)(this->*(load))(context, "vkCmdDispatchBase");
  vkCreateDescriptorUpdateTemplate = (PFN_vkCreateDescriptorUpdateTemplate)(this->*(load))(context, "vkCreateDescriptorUpdateTemplate");
  vkCreateSamplerYcbcrConversion = (PFN_vkCreateSamplerYcbcrConversion)(this->*(load))(context, "vkCreateSamplerYcbcrConversion");
  vkDestroyDescriptorUpdateTemplate = (PFN_vkDestroyDescriptorUpdateTemplate)(this->*(load))(context, "vkDestroyDescriptorUpdateTemplate");
  vkDestroySamplerYcbcrConversion = (PFN_vkDestroySamplerYcbcrConversion)(this->*(load))(context, "vkDestroySamplerYcbcrConversion");
  vkGetDescriptorSetLayoutSupport = (PFN_vkGetDescriptorSetLayoutSupport)(this->*(load))(context, "vkGetDescriptorSetLayoutSupport");
  vkUpdateDescriptorSetWithTemplate = (PFN_vkUpdateDescriptorSetWithTemplate)(this->*(load))(context, "vkUpdateDescriptorSetWithTemplate");
#endif /* defined(VK_COMPUTE_VERSION_1_1) */
#if defined(VK_BASE_VERSION_1_2)
  vkGetBufferDeviceAddress = (PFN_vkGetBufferDeviceAddress)(this->*(load))(context, "vkGetBufferDeviceAddress");
  vkGetBufferOpaqueCaptureAddress = (PFN_vkGetBufferOpaqueCaptureAddress)(this->*(load))(context, "vkGetBufferOpaqueCaptureAddress");
  vkGetDeviceMemoryOpaqueCaptureAddress = (PFN_vkGetDeviceMemoryOpaqueCaptureAddress)(this->*(load))(context, "vkGetDeviceMemoryOpaqueCaptureAddress");
  vkGetSemaphoreCounterValue = (PFN_vkGetSemaphoreCounterValue)(this->*(load))(context, "vkGetSemaphoreCounterValue");
  vkResetQueryPool = (PFN_vkResetQueryPool)(this->*(load))(context, "vkResetQueryPool");
  vkSignalSemaphore = (PFN_vkSignalSemaphore)(this->*(load))(context, "vkSignalSemaphore");
  vkWaitSemaphores = (PFN_vkWaitSemaphores)(this->*(load))(context, "vkWaitSemaphores");
#endif /* defined(VK_BASE_VERSION_1_2) */
#if defined(VK_GRAPHICS_VERSION_1_2)
  vkCmdBeginRenderPass2 = (PFN_vkCmdBeginRenderPass2)(this->*(load))(context, "vkCmdBeginRenderPass2");
  vkCmdDrawIndexedIndirectCount = (PFN_vkCmdDrawIndexedIndirectCount)(this->*(load))(context, "vkCmdDrawIndexedIndirectCount");
  vkCmdDrawIndirectCount = (PFN_vkCmdDrawIndirectCount)(this->*(load))(context, "vkCmdDrawIndirectCount");
  vkCmdEndRenderPass2 = (PFN_vkCmdEndRenderPass2)(this->*(load))(context, "vkCmdEndRenderPass2");
  vkCmdNextSubpass2 = (PFN_vkCmdNextSubpass2)(this->*(load))(context, "vkCmdNextSubpass2");
  vkCreateRenderPass2 = (PFN_vkCreateRenderPass2)(this->*(load))(context, "vkCreateRenderPass2");
#endif /* defined(VK_GRAPHICS_VERSION_1_2) */
#if defined(VK_BASE_VERSION_1_3)
  vkCmdCopyBuffer2 = (PFN_vkCmdCopyBuffer2)(this->*(load))(context, "vkCmdCopyBuffer2");
  vkCmdCopyBufferToImage2 = (PFN_vkCmdCopyBufferToImage2)(this->*(load))(context, "vkCmdCopyBufferToImage2");
  vkCmdCopyImage2 = (PFN_vkCmdCopyImage2)(this->*(load))(context, "vkCmdCopyImage2");
  vkCmdCopyImageToBuffer2 = (PFN_vkCmdCopyImageToBuffer2)(this->*(load))(context, "vkCmdCopyImageToBuffer2");
  vkCmdPipelineBarrier2 = (PFN_vkCmdPipelineBarrier2)(this->*(load))(context, "vkCmdPipelineBarrier2");
  vkCmdWriteTimestamp2 = (PFN_vkCmdWriteTimestamp2)(this->*(load))(context, "vkCmdWriteTimestamp2");
  vkCreatePrivateDataSlot = (PFN_vkCreatePrivateDataSlot)(this->*(load))(context, "vkCreatePrivateDataSlot");
  vkDestroyPrivateDataSlot = (PFN_vkDestroyPrivateDataSlot)(this->*(load))(context, "vkDestroyPrivateDataSlot");
  vkGetDeviceBufferMemoryRequirements = (PFN_vkGetDeviceBufferMemoryRequirements)(this->*(load))(context, "vkGetDeviceBufferMemoryRequirements");
  vkGetDeviceImageMemoryRequirements = (PFN_vkGetDeviceImageMemoryRequirements)(this->*(load))(context, "vkGetDeviceImageMemoryRequirements");
  vkGetDeviceImageSparseMemoryRequirements = (PFN_vkGetDeviceImageSparseMemoryRequirements)(this->*(load))(context, "vkGetDeviceImageSparseMemoryRequirements");
  vkGetPrivateData = (PFN_vkGetPrivateData)(this->*(load))(context, "vkGetPrivateData");
  vkQueueSubmit2 = (PFN_vkQueueSubmit2)(this->*(load))(context, "vkQueueSubmit2");
  vkSetPrivateData = (PFN_vkSetPrivateData)(this->*(load))(context, "vkSetPrivateData");
#endif /* defined(VK_BASE_VERSION_1_3) */
#if defined(VK_COMPUTE_VERSION_1_3)
  vkCmdResetEvent2 = (PFN_vkCmdResetEvent2)(this->*(load))(context, "vkCmdResetEvent2");
  vkCmdSetEvent2 = (PFN_vkCmdSetEvent2)(this->*(load))(context, "vkCmdSetEvent2");
  vkCmdWaitEvents2 = (PFN_vkCmdWaitEvents2)(this->*(load))(context, "vkCmdWaitEvents2");
#endif /* defined(VK_COMPUTE_VERSION_1_3) */
#if defined(VK_GRAPHICS_VERSION_1_3)
  vkCmdBeginRendering = (PFN_vkCmdBeginRendering)(this->*(load))(context, "vkCmdBeginRendering");
  vkCmdBindVertexBuffers2 = (PFN_vkCmdBindVertexBuffers2)(this->*(load))(context, "vkCmdBindVertexBuffers2");
  vkCmdBlitImage2 = (PFN_vkCmdBlitImage2)(this->*(load))(context, "vkCmdBlitImage2");
  vkCmdEndRendering = (PFN_vkCmdEndRendering)(this->*(load))(context, "vkCmdEndRendering");
  vkCmdResolveImage2 = (PFN_vkCmdResolveImage2)(this->*(load))(context, "vkCmdResolveImage2");
  vkCmdSetCullMode = (PFN_vkCmdSetCullMode)(this->*(load))(context, "vkCmdSetCullMode");
  vkCmdSetDepthBiasEnable = (PFN_vkCmdSetDepthBiasEnable)(this->*(load))(context, "vkCmdSetDepthBiasEnable");
  vkCmdSetDepthBoundsTestEnable = (PFN_vkCmdSetDepthBoundsTestEnable)(this->*(load))(context, "vkCmdSetDepthBoundsTestEnable");
  vkCmdSetDepthCompareOp = (PFN_vkCmdSetDepthCompareOp)(this->*(load))(context, "vkCmdSetDepthCompareOp");
  vkCmdSetDepthTestEnable = (PFN_vkCmdSetDepthTestEnable)(this->*(load))(context, "vkCmdSetDepthTestEnable");
  vkCmdSetDepthWriteEnable = (PFN_vkCmdSetDepthWriteEnable)(this->*(load))(context, "vkCmdSetDepthWriteEnable");
  vkCmdSetFrontFace = (PFN_vkCmdSetFrontFace)(this->*(load))(context, "vkCmdSetFrontFace");
  vkCmdSetPrimitiveRestartEnable = (PFN_vkCmdSetPrimitiveRestartEnable)(this->*(load))(context, "vkCmdSetPrimitiveRestartEnable");
  vkCmdSetPrimitiveTopology = (PFN_vkCmdSetPrimitiveTopology)(this->*(load))(context, "vkCmdSetPrimitiveTopology");
  vkCmdSetRasterizerDiscardEnable = (PFN_vkCmdSetRasterizerDiscardEnable)(this->*(load))(context, "vkCmdSetRasterizerDiscardEnable");
  vkCmdSetScissorWithCount = (PFN_vkCmdSetScissorWithCount)(this->*(load))(context, "vkCmdSetScissorWithCount");
  vkCmdSetStencilOp = (PFN_vkCmdSetStencilOp)(this->*(load))(context, "vkCmdSetStencilOp");
  vkCmdSetStencilTestEnable = (PFN_vkCmdSetStencilTestEnable)(this->*(load))(context, "vkCmdSetStencilTestEnable");
  vkCmdSetViewportWithCount = (PFN_vkCmdSetViewportWithCount)(this->*(load))(context, "vkCmdSetViewportWithCount");
#endif /* defined(VK_GRAPHICS_VERSION_1_3) */
#if defined(VK_BASE_VERSION_1_4)
  vkCopyImageToImage = (PFN_vkCopyImageToImage)(this->*(load))(context, "vkCopyImageToImage");
  vkCopyImageToMemory = (PFN_vkCopyImageToMemory)(this->*(load))(context, "vkCopyImageToMemory");
  vkCopyMemoryToImage = (PFN_vkCopyMemoryToImage)(this->*(load))(context, "vkCopyMemoryToImage");
  vkGetDeviceImageSubresourceLayout = (PFN_vkGetDeviceImageSubresourceLayout)(this->*(load))(context, "vkGetDeviceImageSubresourceLayout");
  vkGetImageSubresourceLayout2 = (PFN_vkGetImageSubresourceLayout2)(this->*(load))(context, "vkGetImageSubresourceLayout2");
  vkMapMemory2 = (PFN_vkMapMemory2)(this->*(load))(context, "vkMapMemory2");
  vkTransitionImageLayout = (PFN_vkTransitionImageLayout)(this->*(load))(context, "vkTransitionImageLayout");
  vkUnmapMemory2 = (PFN_vkUnmapMemory2)(this->*(load))(context, "vkUnmapMemory2");
#endif /* defined(VK_BASE_VERSION_1_4) */
#if defined(VK_COMPUTE_VERSION_1_4)
  vkCmdBindDescriptorSets2 = (PFN_vkCmdBindDescriptorSets2)(this->*(load))(context, "vkCmdBindDescriptorSets2");
  vkCmdPushConstants2 = (PFN_vkCmdPushConstants2)(this->*(load))(context, "vkCmdPushConstants2");
  vkCmdPushDescriptorSet = (PFN_vkCmdPushDescriptorSet)(this->*(load))(context, "vkCmdPushDescriptorSet");
  vkCmdPushDescriptorSet2 = (PFN_vkCmdPushDescriptorSet2)(this->*(load))(context, "vkCmdPushDescriptorSet2");
  vkCmdPushDescriptorSetWithTemplate = (PFN_vkCmdPushDescriptorSetWithTemplate)(this->*(load))(context, "vkCmdPushDescriptorSetWithTemplate");
  vkCmdPushDescriptorSetWithTemplate2 = (PFN_vkCmdPushDescriptorSetWithTemplate2)(this->*(load))(context, "vkCmdPushDescriptorSetWithTemplate2");
#endif /* defined(VK_COMPUTE_VERSION_1_4) */
#if defined(VK_GRAPHICS_VERSION_1_4)
  vkCmdBindIndexBuffer2 = (PFN_vkCmdBindIndexBuffer2)(this->*(load))(context, "vkCmdBindIndexBuffer2");
  vkCmdSetLineStipple = (PFN_vkCmdSetLineStipple)(this->*(load))(context, "vkCmdSetLineStipple");
  vkCmdSetRenderingAttachmentLocations = (PFN_vkCmdSetRenderingAttachmentLocations)(this->*(load))(context, "vkCmdSetRenderingAttachmentLocations");
  vkCmdSetRenderingInputAttachmentIndices = (PFN_vkCmdSetRenderingInputAttachmentIndices)(this->*(load))(context, "vkCmdSetRenderingInputAttachmentIndices");
  vkGetRenderingAreaGranularity = (PFN_vkGetRenderingAreaGranularity)(this->*(load))(context, "vkGetRenderingAreaGranularity");
#endif /* defined(VK_GRAPHICS_VERSION_1_4) */
#if defined(VK_AMDX_shader_enqueue)
  vkCmdDispatchGraphAMDX = (PFN_vkCmdDispatchGraphAMDX)(this->*(load))(context, "vkCmdDispatchGraphAMDX");
  vkCmdDispatchGraphIndirectAMDX = (PFN_vkCmdDispatchGraphIndirectAMDX)(this->*(load))(context, "vkCmdDispatchGraphIndirectAMDX");
  vkCmdDispatchGraphIndirectCountAMDX = (PFN_vkCmdDispatchGraphIndirectCountAMDX)(this->*(load))(context, "vkCmdDispatchGraphIndirectCountAMDX");
  vkCmdInitializeGraphScratchMemoryAMDX = (PFN_vkCmdInitializeGraphScratchMemoryAMDX)(this->*(load))(context, "vkCmdInitializeGraphScratchMemoryAMDX");
  vkCreateExecutionGraphPipelinesAMDX = (PFN_vkCreateExecutionGraphPipelinesAMDX)(this->*(load))(context, "vkCreateExecutionGraphPipelinesAMDX");
  vkGetExecutionGraphPipelineNodeIndexAMDX = (PFN_vkGetExecutionGraphPipelineNodeIndexAMDX)(this->*(load))(context, "vkGetExecutionGraphPipelineNodeIndexAMDX");
  vkGetExecutionGraphPipelineScratchSizeAMDX = (PFN_vkGetExecutionGraphPipelineScratchSizeAMDX)(this->*(load))(context, "vkGetExecutionGraphPipelineScratchSizeAMDX");
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_anti_lag)
  vkAntiLagUpdateAMD = (PFN_vkAntiLagUpdateAMD)(this->*(load))(context, "vkAntiLagUpdateAMD");
#endif /* defined(VK_AMD_anti_lag) */
#if defined(VK_AMD_buffer_marker)
  vkCmdWriteBufferMarkerAMD = (PFN_vkCmdWriteBufferMarkerAMD)(this->*(load))(context, "vkCmdWriteBufferMarkerAMD");
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
  vkCmdWriteBufferMarker2AMD = (PFN_vkCmdWriteBufferMarker2AMD)(this->*(load))(context, "vkCmdWriteBufferMarker2AMD");
#endif /* defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) */
#if defined(VK_AMD_display_native_hdr)
  vkSetLocalDimmingAMD = (PFN_vkSetLocalDimmingAMD)(this->*(load))(context, "vkSetLocalDimmingAMD");
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
  vkCmdDrawIndexedIndirectCountAMD = (PFN_vkCmdDrawIndexedIndirectCountAMD)(this->*(load))(context, "vkCmdDrawIndexedIndirectCountAMD");
  vkCmdDrawIndirectCountAMD = (PFN_vkCmdDrawIndirectCountAMD)(this->*(load))(context, "vkCmdDrawIndirectCountAMD");
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
  vkGetShaderInfoAMD = (PFN_vkGetShaderInfoAMD)(this->*(load))(context, "vkGetShaderInfoAMD");
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
  vkGetAndroidHardwareBufferPropertiesANDROID = (PFN_vkGetAndroidHardwareBufferPropertiesANDROID)(this->*(load))(context, "vkGetAndroidHardwareBufferPropertiesANDROID");
  vkGetMemoryAndroidHardwareBufferANDROID = (PFN_vkGetMemoryAndroidHardwareBufferANDROID)(this->*(load))(context, "vkGetMemoryAndroidHardwareBufferANDROID");
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_ARM_data_graph)
  vkBindDataGraphPipelineSessionMemoryARM = (PFN_vkBindDataGraphPipelineSessionMemoryARM)(this->*(load))(context, "vkBindDataGraphPipelineSessionMemoryARM");
  vkCmdDispatchDataGraphARM = (PFN_vkCmdDispatchDataGraphARM)(this->*(load))(context, "vkCmdDispatchDataGraphARM");
  vkCreateDataGraphPipelineSessionARM = (PFN_vkCreateDataGraphPipelineSessionARM)(this->*(load))(context, "vkCreateDataGraphPipelineSessionARM");
  vkCreateDataGraphPipelinesARM = (PFN_vkCreateDataGraphPipelinesARM)(this->*(load))(context, "vkCreateDataGraphPipelinesARM");
  vkDestroyDataGraphPipelineSessionARM = (PFN_vkDestroyDataGraphPipelineSessionARM)(this->*(load))(context, "vkDestroyDataGraphPipelineSessionARM");
  vkGetDataGraphPipelineAvailablePropertiesARM = (PFN_vkGetDataGraphPipelineAvailablePropertiesARM)(this->*(load))(context, "vkGetDataGraphPipelineAvailablePropertiesARM");
  vkGetDataGraphPipelinePropertiesARM = (PFN_vkGetDataGraphPipelinePropertiesARM)(this->*(load))(context, "vkGetDataGraphPipelinePropertiesARM");
  vkGetDataGraphPipelineSessionBindPointRequirementsARM = (PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM)(this->*(load))(context, "vkGetDataGraphPipelineSessionBindPointRequirementsARM");
  vkGetDataGraphPipelineSessionMemoryRequirementsARM = (PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM)(this->*(load))(context, "vkGetDataGraphPipelineSessionMemoryRequirementsARM");
#endif /* defined(VK_ARM_data_graph) */
#if defined(VK_ARM_tensors)
  vkBindTensorMemoryARM = (PFN_vkBindTensorMemoryARM)(this->*(load))(context, "vkBindTensorMemoryARM");
  vkCmdCopyTensorARM = (PFN_vkCmdCopyTensorARM)(this->*(load))(context, "vkCmdCopyTensorARM");
  vkCreateTensorARM = (PFN_vkCreateTensorARM)(this->*(load))(context, "vkCreateTensorARM");
  vkCreateTensorViewARM = (PFN_vkCreateTensorViewARM)(this->*(load))(context, "vkCreateTensorViewARM");
  vkDestroyTensorARM = (PFN_vkDestroyTensorARM)(this->*(load))(context, "vkDestroyTensorARM");
  vkDestroyTensorViewARM = (PFN_vkDestroyTensorViewARM)(this->*(load))(context, "vkDestroyTensorViewARM");
  vkGetDeviceTensorMemoryRequirementsARM = (PFN_vkGetDeviceTensorMemoryRequirementsARM)(this->*(load))(context, "vkGetDeviceTensorMemoryRequirementsARM");
  vkGetTensorMemoryRequirementsARM = (PFN_vkGetTensorMemoryRequirementsARM)(this->*(load))(context, "vkGetTensorMemoryRequirementsARM");
#endif /* defined(VK_ARM_tensors) */
#if defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer)
  vkGetTensorOpaqueCaptureDescriptorDataARM = (PFN_vkGetTensorOpaqueCaptureDescriptorDataARM)(this->*(load))(context, "vkGetTensorOpaqueCaptureDescriptorDataARM");
  vkGetTensorViewOpaqueCaptureDescriptorDataARM = (PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM)(this->*(load))(context, "vkGetTensorViewOpaqueCaptureDescriptorDataARM");
#endif /* defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
  vkCmdSetAttachmentFeedbackLoopEnableEXT = (PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT)(this->*(load))(context, "vkCmdSetAttachmentFeedbackLoopEnableEXT");
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
  vkGetBufferDeviceAddressEXT = (PFN_vkGetBufferDeviceAddressEXT)(this->*(load))(context, "vkGetBufferDeviceAddressEXT");
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
  vkGetCalibratedTimestampsEXT = (PFN_vkGetCalibratedTimestampsEXT)(this->*(load))(context, "vkGetCalibratedTimestampsEXT");
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
  vkCmdSetColorWriteEnableEXT = (PFN_vkCmdSetColorWriteEnableEXT)(this->*(load))(context, "vkCmdSetColorWriteEnableEXT");
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
  vkCmdBeginConditionalRenderingEXT = (PFN_vkCmdBeginConditionalRenderingEXT)(this->*(load))(context, "vkCmdBeginConditionalRenderingEXT");
  vkCmdEndConditionalRenderingEXT = (PFN_vkCmdEndConditionalRenderingEXT)(this->*(load))(context, "vkCmdEndConditionalRenderingEXT");
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))
  vkCmdBeginCustomResolveEXT = (PFN_vkCmdBeginCustomResolveEXT)(this->*(load))(context, "vkCmdBeginCustomResolveEXT");
#endif /* defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)) */
#if defined(VK_EXT_debug_marker)
  vkCmdDebugMarkerBeginEXT = (PFN_vkCmdDebugMarkerBeginEXT)(this->*(load))(context, "vkCmdDebugMarkerBeginEXT");
  vkCmdDebugMarkerEndEXT = (PFN_vkCmdDebugMarkerEndEXT)(this->*(load))(context, "vkCmdDebugMarkerEndEXT");
  vkCmdDebugMarkerInsertEXT = (PFN_vkCmdDebugMarkerInsertEXT)(this->*(load))(context, "vkCmdDebugMarkerInsertEXT");
  vkDebugMarkerSetObjectNameEXT = (PFN_vkDebugMarkerSetObjectNameEXT)(this->*(load))(context, "vkDebugMarkerSetObjectNameEXT");
  vkDebugMarkerSetObjectTagEXT = (PFN_vkDebugMarkerSetObjectTagEXT)(this->*(load))(context, "vkDebugMarkerSetObjectTagEXT");
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_depth_bias_control)
  vkCmdSetDepthBias2EXT = (PFN_vkCmdSetDepthBias2EXT)(this->*(load))(context, "vkCmdSetDepthBias2EXT");
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
  vkCmdBindDescriptorBufferEmbeddedSamplersEXT = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT)(this->*(load))(context, "vkCmdBindDescriptorBufferEmbeddedSamplersEXT");
  vkCmdBindDescriptorBuffersEXT = (PFN_vkCmdBindDescriptorBuffersEXT)(this->*(load))(context, "vkCmdBindDescriptorBuffersEXT");
  vkCmdSetDescriptorBufferOffsetsEXT = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)(this->*(load))(context, "vkCmdSetDescriptorBufferOffsetsEXT");
  vkGetBufferOpaqueCaptureDescriptorDataEXT = (PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT)(this->*(load))(context, "vkGetBufferOpaqueCaptureDescriptorDataEXT");
  vkGetDescriptorEXT = (PFN_vkGetDescriptorEXT)(this->*(load))(context, "vkGetDescriptorEXT");
  vkGetDescriptorSetLayoutBindingOffsetEXT = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)(this->*(load))(context, "vkGetDescriptorSetLayoutBindingOffsetEXT");
  vkGetDescriptorSetLayoutSizeEXT = (PFN_vkGetDescriptorSetLayoutSizeEXT)(this->*(load))(context, "vkGetDescriptorSetLayoutSizeEXT");
  vkGetImageOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageOpaqueCaptureDescriptorDataEXT)(this->*(load))(context, "vkGetImageOpaqueCaptureDescriptorDataEXT");
  vkGetImageViewOpaqueCaptureDescriptorDataEXT = (PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT)(this->*(load))(context, "vkGetImageViewOpaqueCaptureDescriptorDataEXT");
  vkGetSamplerOpaqueCaptureDescriptorDataEXT = (PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT)(this->*(load))(context, "vkGetSamplerOpaqueCaptureDescriptorDataEXT");
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
  vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT = (PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT)(this->*(load))(context, "vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT");
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
  vkGetDeviceFaultInfoEXT = (PFN_vkGetDeviceFaultInfoEXT)(this->*(load))(context, "vkGetDeviceFaultInfoEXT");
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_device_generated_commands)
  vkCmdExecuteGeneratedCommandsEXT = (PFN_vkCmdExecuteGeneratedCommandsEXT)(this->*(load))(context, "vkCmdExecuteGeneratedCommandsEXT");
  vkCmdPreprocessGeneratedCommandsEXT = (PFN_vkCmdPreprocessGeneratedCommandsEXT)(this->*(load))(context, "vkCmdPreprocessGeneratedCommandsEXT");
  vkCreateIndirectCommandsLayoutEXT = (PFN_vkCreateIndirectCommandsLayoutEXT)(this->*(load))(context, "vkCreateIndirectCommandsLayoutEXT");
  vkCreateIndirectExecutionSetEXT = (PFN_vkCreateIndirectExecutionSetEXT)(this->*(load))(context, "vkCreateIndirectExecutionSetEXT");
  vkDestroyIndirectCommandsLayoutEXT = (PFN_vkDestroyIndirectCommandsLayoutEXT)(this->*(load))(context, "vkDestroyIndirectCommandsLayoutEXT");
  vkDestroyIndirectExecutionSetEXT = (PFN_vkDestroyIndirectExecutionSetEXT)(this->*(load))(context, "vkDestroyIndirectExecutionSetEXT");
  vkGetGeneratedCommandsMemoryRequirementsEXT = (PFN_vkGetGeneratedCommandsMemoryRequirementsEXT)(this->*(load))(context, "vkGetGeneratedCommandsMemoryRequirementsEXT");
  vkUpdateIndirectExecutionSetPipelineEXT = (PFN_vkUpdateIndirectExecutionSetPipelineEXT)(this->*(load))(context, "vkUpdateIndirectExecutionSetPipelineEXT");
  vkUpdateIndirectExecutionSetShaderEXT = (PFN_vkUpdateIndirectExecutionSetShaderEXT)(this->*(load))(context, "vkUpdateIndirectExecutionSetShaderEXT");
#endif /* defined(VK_EXT_device_generated_commands) */
#if defined(VK_EXT_discard_rectangles)
  vkCmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT)(this->*(load))(context, "vkCmdSetDiscardRectangleEXT");
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
  vkCmdSetDiscardRectangleEnableEXT = (PFN_vkCmdSetDiscardRectangleEnableEXT)(this->*(load))(context, "vkCmdSetDiscardRectangleEnableEXT");
  vkCmdSetDiscardRectangleModeEXT = (PFN_vkCmdSetDiscardRectangleModeEXT)(this->*(load))(context, "vkCmdSetDiscardRectangleModeEXT");
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
  vkDisplayPowerControlEXT = (PFN_vkDisplayPowerControlEXT)(this->*(load))(context, "vkDisplayPowerControlEXT");
  vkGetSwapchainCounterEXT = (PFN_vkGetSwapchainCounterEXT)(this->*(load))(context, "vkGetSwapchainCounterEXT");
  vkRegisterDeviceEventEXT = (PFN_vkRegisterDeviceEventEXT)(this->*(load))(context, "vkRegisterDeviceEventEXT");
  vkRegisterDisplayEventEXT = (PFN_vkRegisterDisplayEventEXT)(this->*(load))(context, "vkRegisterDisplayEventEXT");
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_external_memory_host)
  vkGetMemoryHostPointerPropertiesEXT = (PFN_vkGetMemoryHostPointerPropertiesEXT)(this->*(load))(context, "vkGetMemoryHostPointerPropertiesEXT");
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_external_memory_metal)
  vkGetMemoryMetalHandleEXT = (PFN_vkGetMemoryMetalHandleEXT)(this->*(load))(context, "vkGetMemoryMetalHandleEXT");
  vkGetMemoryMetalHandlePropertiesEXT = (PFN_vkGetMemoryMetalHandlePropertiesEXT)(this->*(load))(context, "vkGetMemoryMetalHandlePropertiesEXT");
#endif /* defined(VK_EXT_external_memory_metal) */
#if defined(VK_EXT_fragment_density_map_offset)
  vkCmdEndRendering2EXT = (PFN_vkCmdEndRendering2EXT)(this->*(load))(context, "vkCmdEndRendering2EXT");
#endif /* defined(VK_EXT_fragment_density_map_offset) */
#if defined(VK_EXT_full_screen_exclusive)
  vkAcquireFullScreenExclusiveModeEXT = (PFN_vkAcquireFullScreenExclusiveModeEXT)(this->*(load))(context, "vkAcquireFullScreenExclusiveModeEXT");
  vkReleaseFullScreenExclusiveModeEXT = (PFN_vkReleaseFullScreenExclusiveModeEXT)(this->*(load))(context, "vkReleaseFullScreenExclusiveModeEXT");
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1))
  vkGetDeviceGroupSurfacePresentModes2EXT = (PFN_vkGetDeviceGroupSurfacePresentModes2EXT)(this->*(load))(context, "vkGetDeviceGroupSurfacePresentModes2EXT");
#endif /* defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) */
#if defined(VK_EXT_hdr_metadata)
  vkSetHdrMetadataEXT = (PFN_vkSetHdrMetadataEXT)(this->*(load))(context, "vkSetHdrMetadataEXT");
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_image_copy)
  vkCopyImageToImageEXT = (PFN_vkCopyImageToImageEXT)(this->*(load))(context, "vkCopyImageToImageEXT");
  vkCopyImageToMemoryEXT = (PFN_vkCopyImageToMemoryEXT)(this->*(load))(context, "vkCopyImageToMemoryEXT");
  vkCopyMemoryToImageEXT = (PFN_vkCopyMemoryToImageEXT)(this->*(load))(context, "vkCopyMemoryToImageEXT");
  vkTransitionImageLayoutEXT = (PFN_vkTransitionImageLayoutEXT)(this->*(load))(context, "vkTransitionImageLayoutEXT");
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
  vkResetQueryPoolEXT = (PFN_vkResetQueryPoolEXT)(this->*(load))(context, "vkResetQueryPoolEXT");
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
  vkGetImageDrmFormatModifierPropertiesEXT = (PFN_vkGetImageDrmFormatModifierPropertiesEXT)(this->*(load))(context, "vkGetImageDrmFormatModifierPropertiesEXT");
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
  vkCmdSetLineStippleEXT = (PFN_vkCmdSetLineStippleEXT)(this->*(load))(context, "vkCmdSetLineStippleEXT");
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_memory_decompression)
  vkCmdDecompressMemoryEXT = (PFN_vkCmdDecompressMemoryEXT)(this->*(load))(context, "vkCmdDecompressMemoryEXT");
  vkCmdDecompressMemoryIndirectCountEXT = (PFN_vkCmdDecompressMemoryIndirectCountEXT)(this->*(load))(context, "vkCmdDecompressMemoryIndirectCountEXT");
#endif /* defined(VK_EXT_memory_decompression) */
#if defined(VK_EXT_mesh_shader)
  vkCmdDrawMeshTasksEXT = (PFN_vkCmdDrawMeshTasksEXT)(this->*(load))(context, "vkCmdDrawMeshTasksEXT");
  vkCmdDrawMeshTasksIndirectEXT = (PFN_vkCmdDrawMeshTasksIndirectEXT)(this->*(load))(context, "vkCmdDrawMeshTasksIndirectEXT");
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
  vkCmdDrawMeshTasksIndirectCountEXT = (PFN_vkCmdDrawMeshTasksIndirectCountEXT)(this->*(load))(context, "vkCmdDrawMeshTasksIndirectCountEXT");
#endif /* defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) */
#if defined(VK_EXT_metal_objects)
  vkExportMetalObjectsEXT = (PFN_vkExportMetalObjectsEXT)(this->*(load))(context, "vkExportMetalObjectsEXT");
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
  vkCmdDrawMultiEXT = (PFN_vkCmdDrawMultiEXT)(this->*(load))(context, "vkCmdDrawMultiEXT");
  vkCmdDrawMultiIndexedEXT = (PFN_vkCmdDrawMultiIndexedEXT)(this->*(load))(context, "vkCmdDrawMultiIndexedEXT");
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
  vkBuildMicromapsEXT = (PFN_vkBuildMicromapsEXT)(this->*(load))(context, "vkBuildMicromapsEXT");
  vkCmdBuildMicromapsEXT = (PFN_vkCmdBuildMicromapsEXT)(this->*(load))(context, "vkCmdBuildMicromapsEXT");
  vkCmdCopyMemoryToMicromapEXT = (PFN_vkCmdCopyMemoryToMicromapEXT)(this->*(load))(context, "vkCmdCopyMemoryToMicromapEXT");
  vkCmdCopyMicromapEXT = (PFN_vkCmdCopyMicromapEXT)(this->*(load))(context, "vkCmdCopyMicromapEXT");
  vkCmdCopyMicromapToMemoryEXT = (PFN_vkCmdCopyMicromapToMemoryEXT)(this->*(load))(context, "vkCmdCopyMicromapToMemoryEXT");
  vkCmdWriteMicromapsPropertiesEXT = (PFN_vkCmdWriteMicromapsPropertiesEXT)(this->*(load))(context, "vkCmdWriteMicromapsPropertiesEXT");
  vkCopyMemoryToMicromapEXT = (PFN_vkCopyMemoryToMicromapEXT)(this->*(load))(context, "vkCopyMemoryToMicromapEXT");
  vkCopyMicromapEXT = (PFN_vkCopyMicromapEXT)(this->*(load))(context, "vkCopyMicromapEXT");
  vkCopyMicromapToMemoryEXT = (PFN_vkCopyMicromapToMemoryEXT)(this->*(load))(context, "vkCopyMicromapToMemoryEXT");
  vkCreateMicromapEXT = (PFN_vkCreateMicromapEXT)(this->*(load))(context, "vkCreateMicromapEXT");
  vkDestroyMicromapEXT = (PFN_vkDestroyMicromapEXT)(this->*(load))(context, "vkDestroyMicromapEXT");
  vkGetDeviceMicromapCompatibilityEXT = (PFN_vkGetDeviceMicromapCompatibilityEXT)(this->*(load))(context, "vkGetDeviceMicromapCompatibilityEXT");
  vkGetMicromapBuildSizesEXT = (PFN_vkGetMicromapBuildSizesEXT)(this->*(load))(context, "vkGetMicromapBuildSizesEXT");
  vkWriteMicromapsPropertiesEXT = (PFN_vkWriteMicromapsPropertiesEXT)(this->*(load))(context, "vkWriteMicromapsPropertiesEXT");
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
  vkSetDeviceMemoryPriorityEXT = (PFN_vkSetDeviceMemoryPriorityEXT)(this->*(load))(context, "vkSetDeviceMemoryPriorityEXT");
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
  vkGetPipelinePropertiesEXT = (PFN_vkGetPipelinePropertiesEXT)(this->*(load))(context, "vkGetPipelinePropertiesEXT");
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_present_timing)
  vkGetPastPresentationTimingEXT = (PFN_vkGetPastPresentationTimingEXT)(this->*(load))(context, "vkGetPastPresentationTimingEXT");
  vkGetSwapchainTimeDomainPropertiesEXT = (PFN_vkGetSwapchainTimeDomainPropertiesEXT)(this->*(load))(context, "vkGetSwapchainTimeDomainPropertiesEXT");
  vkGetSwapchainTimingPropertiesEXT = (PFN_vkGetSwapchainTimingPropertiesEXT)(this->*(load))(context, "vkGetSwapchainTimingPropertiesEXT");
  vkSetSwapchainPresentTimingQueueSizeEXT = (PFN_vkSetSwapchainPresentTimingQueueSizeEXT)(this->*(load))(context, "vkSetSwapchainPresentTimingQueueSizeEXT");
#endif /* defined(VK_EXT_present_timing) */
#if defined(VK_EXT_private_data)
  vkCreatePrivateDataSlotEXT = (PFN_vkCreatePrivateDataSlotEXT)(this->*(load))(context, "vkCreatePrivateDataSlotEXT");
  vkDestroyPrivateDataSlotEXT = (PFN_vkDestroyPrivateDataSlotEXT)(this->*(load))(context, "vkDestroyPrivateDataSlotEXT");
  vkGetPrivateDataEXT = (PFN_vkGetPrivateDataEXT)(this->*(load))(context, "vkGetPrivateDataEXT");
  vkSetPrivateDataEXT = (PFN_vkSetPrivateDataEXT)(this->*(load))(context, "vkSetPrivateDataEXT");
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
  vkCmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT)(this->*(load))(context, "vkCmdSetSampleLocationsEXT");
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
  vkGetShaderModuleCreateInfoIdentifierEXT = (PFN_vkGetShaderModuleCreateInfoIdentifierEXT)(this->*(load))(context, "vkGetShaderModuleCreateInfoIdentifierEXT");
  vkGetShaderModuleIdentifierEXT = (PFN_vkGetShaderModuleIdentifierEXT)(this->*(load))(context, "vkGetShaderModuleIdentifierEXT");
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
  vkCmdBindShadersEXT = (PFN_vkCmdBindShadersEXT)(this->*(load))(context, "vkCmdBindShadersEXT");
  vkCreateShadersEXT = (PFN_vkCreateShadersEXT)(this->*(load))(context, "vkCreateShadersEXT");
  vkDestroyShaderEXT = (PFN_vkDestroyShaderEXT)(this->*(load))(context, "vkDestroyShaderEXT");
  vkGetShaderBinaryDataEXT = (PFN_vkGetShaderBinaryDataEXT)(this->*(load))(context, "vkGetShaderBinaryDataEXT");
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
  vkReleaseSwapchainImagesEXT = (PFN_vkReleaseSwapchainImagesEXT)(this->*(load))(context, "vkReleaseSwapchainImagesEXT");
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_transform_feedback)
  vkCmdBeginQueryIndexedEXT = (PFN_vkCmdBeginQueryIndexedEXT)(this->*(load))(context, "vkCmdBeginQueryIndexedEXT");
  vkCmdBeginTransformFeedbackEXT = (PFN_vkCmdBeginTransformFeedbackEXT)(this->*(load))(context, "vkCmdBeginTransformFeedbackEXT");
  vkCmdBindTransformFeedbackBuffersEXT = (PFN_vkCmdBindTransformFeedbackBuffersEXT)(this->*(load))(context, "vkCmdBindTransformFeedbackBuffersEXT");
  vkCmdDrawIndirectByteCountEXT = (PFN_vkCmdDrawIndirectByteCountEXT)(this->*(load))(context, "vkCmdDrawIndirectByteCountEXT");
  vkCmdEndQueryIndexedEXT = (PFN_vkCmdEndQueryIndexedEXT)(this->*(load))(context, "vkCmdEndQueryIndexedEXT");
  vkCmdEndTransformFeedbackEXT = (PFN_vkCmdEndTransformFeedbackEXT)(this->*(load))(context, "vkCmdEndTransformFeedbackEXT");
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
  vkCreateValidationCacheEXT = (PFN_vkCreateValidationCacheEXT)(this->*(load))(context, "vkCreateValidationCacheEXT");
  vkDestroyValidationCacheEXT = (PFN_vkDestroyValidationCacheEXT)(this->*(load))(context, "vkDestroyValidationCacheEXT");
  vkGetValidationCacheDataEXT = (PFN_vkGetValidationCacheDataEXT)(this->*(load))(context, "vkGetValidationCacheDataEXT");
  vkMergeValidationCachesEXT = (PFN_vkMergeValidationCachesEXT)(this->*(load))(context, "vkMergeValidationCachesEXT");
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
  vkCreateBufferCollectionFUCHSIA = (PFN_vkCreateBufferCollectionFUCHSIA)(this->*(load))(context, "vkCreateBufferCollectionFUCHSIA");
  vkDestroyBufferCollectionFUCHSIA = (PFN_vkDestroyBufferCollectionFUCHSIA)(this->*(load))(context, "vkDestroyBufferCollectionFUCHSIA");
  vkGetBufferCollectionPropertiesFUCHSIA = (PFN_vkGetBufferCollectionPropertiesFUCHSIA)(this->*(load))(context, "vkGetBufferCollectionPropertiesFUCHSIA");
  vkSetBufferCollectionBufferConstraintsFUCHSIA = (PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA)(this->*(load))(context, "vkSetBufferCollectionBufferConstraintsFUCHSIA");
  vkSetBufferCollectionImageConstraintsFUCHSIA = (PFN_vkSetBufferCollectionImageConstraintsFUCHSIA)(this->*(load))(context, "vkSetBufferCollectionImageConstraintsFUCHSIA");
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
  vkGetMemoryZirconHandleFUCHSIA = (PFN_vkGetMemoryZirconHandleFUCHSIA)(this->*(load))(context, "vkGetMemoryZirconHandleFUCHSIA");
  vkGetMemoryZirconHandlePropertiesFUCHSIA = (PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA)(this->*(load))(context, "vkGetMemoryZirconHandlePropertiesFUCHSIA");
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
  vkGetSemaphoreZirconHandleFUCHSIA = (PFN_vkGetSemaphoreZirconHandleFUCHSIA)(this->*(load))(context, "vkGetSemaphoreZirconHandleFUCHSIA");
  vkImportSemaphoreZirconHandleFUCHSIA = (PFN_vkImportSemaphoreZirconHandleFUCHSIA)(this->*(load))(context, "vkImportSemaphoreZirconHandleFUCHSIA");
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
  vkGetPastPresentationTimingGOOGLE = (PFN_vkGetPastPresentationTimingGOOGLE)(this->*(load))(context, "vkGetPastPresentationTimingGOOGLE");
  vkGetRefreshCycleDurationGOOGLE = (PFN_vkGetRefreshCycleDurationGOOGLE)(this->*(load))(context, "vkGetRefreshCycleDurationGOOGLE");
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
  vkCmdDrawClusterHUAWEI = (PFN_vkCmdDrawClusterHUAWEI)(this->*(load))(context, "vkCmdDrawClusterHUAWEI");
  vkCmdDrawClusterIndirectHUAWEI = (PFN_vkCmdDrawClusterIndirectHUAWEI)(this->*(load))(context, "vkCmdDrawClusterIndirectHUAWEI");
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
  vkCmdBindInvocationMaskHUAWEI = (PFN_vkCmdBindInvocationMaskHUAWEI)(this->*(load))(context, "vkCmdBindInvocationMaskHUAWEI");
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
  vkCmdSubpassShadingHUAWEI = (PFN_vkCmdSubpassShadingHUAWEI)(this->*(load))(context, "vkCmdSubpassShadingHUAWEI");
  vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = (PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI)(this->*(load))(context, "vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI");
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
  vkAcquirePerformanceConfigurationINTEL = (PFN_vkAcquirePerformanceConfigurationINTEL)(this->*(load))(context, "vkAcquirePerformanceConfigurationINTEL");
  vkCmdSetPerformanceMarkerINTEL = (PFN_vkCmdSetPerformanceMarkerINTEL)(this->*(load))(context, "vkCmdSetPerformanceMarkerINTEL");
  vkCmdSetPerformanceOverrideINTEL = (PFN_vkCmdSetPerformanceOverrideINTEL)(this->*(load))(context, "vkCmdSetPerformanceOverrideINTEL");
  vkCmdSetPerformanceStreamMarkerINTEL = (PFN_vkCmdSetPerformanceStreamMarkerINTEL)(this->*(load))(context, "vkCmdSetPerformanceStreamMarkerINTEL");
  vkGetPerformanceParameterINTEL = (PFN_vkGetPerformanceParameterINTEL)(this->*(load))(context, "vkGetPerformanceParameterINTEL");
  vkInitializePerformanceApiINTEL = (PFN_vkInitializePerformanceApiINTEL)(this->*(load))(context, "vkInitializePerformanceApiINTEL");
  vkQueueSetPerformanceConfigurationINTEL = (PFN_vkQueueSetPerformanceConfigurationINTEL)(this->*(load))(context, "vkQueueSetPerformanceConfigurationINTEL");
  vkReleasePerformanceConfigurationINTEL = (PFN_vkReleasePerformanceConfigurationINTEL)(this->*(load))(context, "vkReleasePerformanceConfigurationINTEL");
  vkUninitializePerformanceApiINTEL = (PFN_vkUninitializePerformanceApiINTEL)(this->*(load))(context, "vkUninitializePerformanceApiINTEL");
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
  vkBuildAccelerationStructuresKHR = (PFN_vkBuildAccelerationStructuresKHR)(this->*(load))(context, "vkBuildAccelerationStructuresKHR");
  vkCmdBuildAccelerationStructuresIndirectKHR = (PFN_vkCmdBuildAccelerationStructuresIndirectKHR)(this->*(load))(context, "vkCmdBuildAccelerationStructuresIndirectKHR");
  vkCmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR)(this->*(load))(context, "vkCmdBuildAccelerationStructuresKHR");
  vkCmdCopyAccelerationStructureKHR = (PFN_vkCmdCopyAccelerationStructureKHR)(this->*(load))(context, "vkCmdCopyAccelerationStructureKHR");
  vkCmdCopyAccelerationStructureToMemoryKHR = (PFN_vkCmdCopyAccelerationStructureToMemoryKHR)(this->*(load))(context, "vkCmdCopyAccelerationStructureToMemoryKHR");
  vkCmdCopyMemoryToAccelerationStructureKHR = (PFN_vkCmdCopyMemoryToAccelerationStructureKHR)(this->*(load))(context, "vkCmdCopyMemoryToAccelerationStructureKHR");
  vkCmdWriteAccelerationStructuresPropertiesKHR = (PFN_vkCmdWriteAccelerationStructuresPropertiesKHR)(this->*(load))(context, "vkCmdWriteAccelerationStructuresPropertiesKHR");
  vkCopyAccelerationStructureKHR = (PFN_vkCopyAccelerationStructureKHR)(this->*(load))(context, "vkCopyAccelerationStructureKHR");
  vkCopyAccelerationStructureToMemoryKHR = (PFN_vkCopyAccelerationStructureToMemoryKHR)(this->*(load))(context, "vkCopyAccelerationStructureToMemoryKHR");
  vkCopyMemoryToAccelerationStructureKHR = (PFN_vkCopyMemoryToAccelerationStructureKHR)(this->*(load))(context, "vkCopyMemoryToAccelerationStructureKHR");
  vkCreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR)(this->*(load))(context, "vkCreateAccelerationStructureKHR");
  vkDestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR)(this->*(load))(context, "vkDestroyAccelerationStructureKHR");
  vkGetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR)(this->*(load))(context, "vkGetAccelerationStructureBuildSizesKHR");
  vkGetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR)(this->*(load))(context, "vkGetAccelerationStructureDeviceAddressKHR");
  vkGetDeviceAccelerationStructureCompatibilityKHR = (PFN_vkGetDeviceAccelerationStructureCompatibilityKHR)(this->*(load))(context, "vkGetDeviceAccelerationStructureCompatibilityKHR");
  vkWriteAccelerationStructuresPropertiesKHR = (PFN_vkWriteAccelerationStructuresPropertiesKHR)(this->*(load))(context, "vkWriteAccelerationStructuresPropertiesKHR");
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
  vkBindBufferMemory2KHR = (PFN_vkBindBufferMemory2KHR)(this->*(load))(context, "vkBindBufferMemory2KHR");
  vkBindImageMemory2KHR = (PFN_vkBindImageMemory2KHR)(this->*(load))(context, "vkBindImageMemory2KHR");
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
  vkGetBufferDeviceAddressKHR = (PFN_vkGetBufferDeviceAddressKHR)(this->*(load))(context, "vkGetBufferDeviceAddressKHR");
  vkGetBufferOpaqueCaptureAddressKHR = (PFN_vkGetBufferOpaqueCaptureAddressKHR)(this->*(load))(context, "vkGetBufferOpaqueCaptureAddressKHR");
  vkGetDeviceMemoryOpaqueCaptureAddressKHR = (PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)(this->*(load))(context, "vkGetDeviceMemoryOpaqueCaptureAddressKHR");
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
  vkGetCalibratedTimestampsKHR = (PFN_vkGetCalibratedTimestampsKHR)(this->*(load))(context, "vkGetCalibratedTimestampsKHR");
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_copy_commands2)
  vkCmdBlitImage2KHR = (PFN_vkCmdBlitImage2KHR)(this->*(load))(context, "vkCmdBlitImage2KHR");
  vkCmdCopyBuffer2KHR = (PFN_vkCmdCopyBuffer2KHR)(this->*(load))(context, "vkCmdCopyBuffer2KHR");
  vkCmdCopyBufferToImage2KHR = (PFN_vkCmdCopyBufferToImage2KHR)(this->*(load))(context, "vkCmdCopyBufferToImage2KHR");
  vkCmdCopyImage2KHR = (PFN_vkCmdCopyImage2KHR)(this->*(load))(context, "vkCmdCopyImage2KHR");
  vkCmdCopyImageToBuffer2KHR = (PFN_vkCmdCopyImageToBuffer2KHR)(this->*(load))(context, "vkCmdCopyImageToBuffer2KHR");
  vkCmdResolveImage2KHR = (PFN_vkCmdResolveImage2KHR)(this->*(load))(context, "vkCmdResolveImage2KHR");
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_copy_memory_indirect)
  vkCmdCopyMemoryIndirectKHR = (PFN_vkCmdCopyMemoryIndirectKHR)(this->*(load))(context, "vkCmdCopyMemoryIndirectKHR");
  vkCmdCopyMemoryToImageIndirectKHR = (PFN_vkCmdCopyMemoryToImageIndirectKHR)(this->*(load))(context, "vkCmdCopyMemoryToImageIndirectKHR");
#endif /* defined(VK_KHR_copy_memory_indirect) */
#if defined(VK_KHR_create_renderpass2)
  vkCmdBeginRenderPass2KHR = (PFN_vkCmdBeginRenderPass2KHR)(this->*(load))(context, "vkCmdBeginRenderPass2KHR");
  vkCmdEndRenderPass2KHR = (PFN_vkCmdEndRenderPass2KHR)(this->*(load))(context, "vkCmdEndRenderPass2KHR");
  vkCmdNextSubpass2KHR = (PFN_vkCmdNextSubpass2KHR)(this->*(load))(context, "vkCmdNextSubpass2KHR");
  vkCreateRenderPass2KHR = (PFN_vkCreateRenderPass2KHR)(this->*(load))(context, "vkCreateRenderPass2KHR");
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
  vkCreateDeferredOperationKHR = (PFN_vkCreateDeferredOperationKHR)(this->*(load))(context, "vkCreateDeferredOperationKHR");
  vkDeferredOperationJoinKHR = (PFN_vkDeferredOperationJoinKHR)(this->*(load))(context, "vkDeferredOperationJoinKHR");
  vkDestroyDeferredOperationKHR = (PFN_vkDestroyDeferredOperationKHR)(this->*(load))(context, "vkDestroyDeferredOperationKHR");
  vkGetDeferredOperationMaxConcurrencyKHR = (PFN_vkGetDeferredOperationMaxConcurrencyKHR)(this->*(load))(context, "vkGetDeferredOperationMaxConcurrencyKHR");
  vkGetDeferredOperationResultKHR = (PFN_vkGetDeferredOperationResultKHR)(this->*(load))(context, "vkGetDeferredOperationResultKHR");
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
  vkCreateDescriptorUpdateTemplateKHR = (PFN_vkCreateDescriptorUpdateTemplateKHR)(this->*(load))(context, "vkCreateDescriptorUpdateTemplateKHR");
  vkDestroyDescriptorUpdateTemplateKHR = (PFN_vkDestroyDescriptorUpdateTemplateKHR)(this->*(load))(context, "vkDestroyDescriptorUpdateTemplateKHR");
  vkUpdateDescriptorSetWithTemplateKHR = (PFN_vkUpdateDescriptorSetWithTemplateKHR)(this->*(load))(context, "vkUpdateDescriptorSetWithTemplateKHR");
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
  vkCmdDispatchBaseKHR = (PFN_vkCmdDispatchBaseKHR)(this->*(load))(context, "vkCmdDispatchBaseKHR");
  vkCmdSetDeviceMaskKHR = (PFN_vkCmdSetDeviceMaskKHR)(this->*(load))(context, "vkCmdSetDeviceMaskKHR");
  vkGetDeviceGroupPeerMemoryFeaturesKHR = (PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR)(this->*(load))(context, "vkGetDeviceGroupPeerMemoryFeaturesKHR");
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
  vkCreateSharedSwapchainsKHR = (PFN_vkCreateSharedSwapchainsKHR)(this->*(load))(context, "vkCreateSharedSwapchainsKHR");
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
  vkCmdDrawIndexedIndirectCountKHR = (PFN_vkCmdDrawIndexedIndirectCountKHR)(this->*(load))(context, "vkCmdDrawIndexedIndirectCountKHR");
  vkCmdDrawIndirectCountKHR = (PFN_vkCmdDrawIndirectCountKHR)(this->*(load))(context, "vkCmdDrawIndirectCountKHR");
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
  vkCmdBeginRenderingKHR = (PFN_vkCmdBeginRenderingKHR)(this->*(load))(context, "vkCmdBeginRenderingKHR");
  vkCmdEndRenderingKHR = (PFN_vkCmdEndRenderingKHR)(this->*(load))(context, "vkCmdEndRenderingKHR");
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
  vkCmdSetRenderingAttachmentLocationsKHR = (PFN_vkCmdSetRenderingAttachmentLocationsKHR)(this->*(load))(context, "vkCmdSetRenderingAttachmentLocationsKHR");
  vkCmdSetRenderingInputAttachmentIndicesKHR = (PFN_vkCmdSetRenderingInputAttachmentIndicesKHR)(this->*(load))(context, "vkCmdSetRenderingInputAttachmentIndicesKHR");
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_fd)
  vkGetFenceFdKHR = (PFN_vkGetFenceFdKHR)(this->*(load))(context, "vkGetFenceFdKHR");
  vkImportFenceFdKHR = (PFN_vkImportFenceFdKHR)(this->*(load))(context, "vkImportFenceFdKHR");
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
  vkGetFenceWin32HandleKHR = (PFN_vkGetFenceWin32HandleKHR)(this->*(load))(context, "vkGetFenceWin32HandleKHR");
  vkImportFenceWin32HandleKHR = (PFN_vkImportFenceWin32HandleKHR)(this->*(load))(context, "vkImportFenceWin32HandleKHR");
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
  vkGetMemoryFdKHR = (PFN_vkGetMemoryFdKHR)(this->*(load))(context, "vkGetMemoryFdKHR");
  vkGetMemoryFdPropertiesKHR = (PFN_vkGetMemoryFdPropertiesKHR)(this->*(load))(context, "vkGetMemoryFdPropertiesKHR");
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
  vkGetMemoryWin32HandleKHR = (PFN_vkGetMemoryWin32HandleKHR)(this->*(load))(context, "vkGetMemoryWin32HandleKHR");
  vkGetMemoryWin32HandlePropertiesKHR = (PFN_vkGetMemoryWin32HandlePropertiesKHR)(this->*(load))(context, "vkGetMemoryWin32HandlePropertiesKHR");
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
  vkGetSemaphoreFdKHR = (PFN_vkGetSemaphoreFdKHR)(this->*(load))(context, "vkGetSemaphoreFdKHR");
  vkImportSemaphoreFdKHR = (PFN_vkImportSemaphoreFdKHR)(this->*(load))(context, "vkImportSemaphoreFdKHR");
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
  vkGetSemaphoreWin32HandleKHR = (PFN_vkGetSemaphoreWin32HandleKHR)(this->*(load))(context, "vkGetSemaphoreWin32HandleKHR");
  vkImportSemaphoreWin32HandleKHR = (PFN_vkImportSemaphoreWin32HandleKHR)(this->*(load))(context, "vkImportSemaphoreWin32HandleKHR");
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
  vkCmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR)(this->*(load))(context, "vkCmdSetFragmentShadingRateKHR");
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
  vkGetBufferMemoryRequirements2KHR = (PFN_vkGetBufferMemoryRequirements2KHR)(this->*(load))(context, "vkGetBufferMemoryRequirements2KHR");
  vkGetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR)(this->*(load))(context, "vkGetImageMemoryRequirements2KHR");
  vkGetImageSparseMemoryRequirements2KHR = (PFN_vkGetImageSparseMemoryRequirements2KHR)(this->*(load))(context, "vkGetImageSparseMemoryRequirements2KHR");
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_line_rasterization)
  vkCmdSetLineStippleKHR = (PFN_vkCmdSetLineStippleKHR)(this->*(load))(context, "vkCmdSetLineStippleKHR");
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
  vkTrimCommandPoolKHR = (PFN_vkTrimCommandPoolKHR)(this->*(load))(context, "vkTrimCommandPoolKHR");
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance10)
  vkCmdEndRendering2KHR = (PFN_vkCmdEndRendering2KHR)(this->*(load))(context, "vkCmdEndRendering2KHR");
#endif /* defined(VK_KHR_maintenance10) */
#if defined(VK_KHR_maintenance3)
  vkGetDescriptorSetLayoutSupportKHR = (PFN_vkGetDescriptorSetLayoutSupportKHR)(this->*(load))(context, "vkGetDescriptorSetLayoutSupportKHR");
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
  vkGetDeviceBufferMemoryRequirementsKHR = (PFN_vkGetDeviceBufferMemoryRequirementsKHR)(this->*(load))(context, "vkGetDeviceBufferMemoryRequirementsKHR");
  vkGetDeviceImageMemoryRequirementsKHR = (PFN_vkGetDeviceImageMemoryRequirementsKHR)(this->*(load))(context, "vkGetDeviceImageMemoryRequirementsKHR");
  vkGetDeviceImageSparseMemoryRequirementsKHR = (PFN_vkGetDeviceImageSparseMemoryRequirementsKHR)(this->*(load))(context, "vkGetDeviceImageSparseMemoryRequirementsKHR");
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
  vkCmdBindIndexBuffer2KHR = (PFN_vkCmdBindIndexBuffer2KHR)(this->*(load))(context, "vkCmdBindIndexBuffer2KHR");
  vkGetDeviceImageSubresourceLayoutKHR = (PFN_vkGetDeviceImageSubresourceLayoutKHR)(this->*(load))(context, "vkGetDeviceImageSubresourceLayoutKHR");
  vkGetImageSubresourceLayout2KHR = (PFN_vkGetImageSubresourceLayout2KHR)(this->*(load))(context, "vkGetImageSubresourceLayout2KHR");
  vkGetRenderingAreaGranularityKHR = (PFN_vkGetRenderingAreaGranularityKHR)(this->*(load))(context, "vkGetRenderingAreaGranularityKHR");
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
  vkCmdBindDescriptorSets2KHR = (PFN_vkCmdBindDescriptorSets2KHR)(this->*(load))(context, "vkCmdBindDescriptorSets2KHR");
  vkCmdPushConstants2KHR = (PFN_vkCmdPushConstants2KHR)(this->*(load))(context, "vkCmdPushConstants2KHR");
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
  vkCmdPushDescriptorSet2KHR = (PFN_vkCmdPushDescriptorSet2KHR)(this->*(load))(context, "vkCmdPushDescriptorSet2KHR");
  vkCmdPushDescriptorSetWithTemplate2KHR = (PFN_vkCmdPushDescriptorSetWithTemplate2KHR)(this->*(load))(context, "vkCmdPushDescriptorSetWithTemplate2KHR");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
  vkCmdBindDescriptorBufferEmbeddedSamplers2EXT = (PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT)(this->*(load))(context, "vkCmdBindDescriptorBufferEmbeddedSamplers2EXT");
  vkCmdSetDescriptorBufferOffsets2EXT = (PFN_vkCmdSetDescriptorBufferOffsets2EXT)(this->*(load))(context, "vkCmdSetDescriptorBufferOffsets2EXT");
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
  vkMapMemory2KHR = (PFN_vkMapMemory2KHR)(this->*(load))(context, "vkMapMemory2KHR");
  vkUnmapMemory2KHR = (PFN_vkUnmapMemory2KHR)(this->*(load))(context, "vkUnmapMemory2KHR");
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
  vkAcquireProfilingLockKHR = (PFN_vkAcquireProfilingLockKHR)(this->*(load))(context, "vkAcquireProfilingLockKHR");
  vkReleaseProfilingLockKHR = (PFN_vkReleaseProfilingLockKHR)(this->*(load))(context, "vkReleaseProfilingLockKHR");
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_binary)
  vkCreatePipelineBinariesKHR = (PFN_vkCreatePipelineBinariesKHR)(this->*(load))(context, "vkCreatePipelineBinariesKHR");
  vkDestroyPipelineBinaryKHR = (PFN_vkDestroyPipelineBinaryKHR)(this->*(load))(context, "vkDestroyPipelineBinaryKHR");
  vkGetPipelineBinaryDataKHR = (PFN_vkGetPipelineBinaryDataKHR)(this->*(load))(context, "vkGetPipelineBinaryDataKHR");
  vkGetPipelineKeyKHR = (PFN_vkGetPipelineKeyKHR)(this->*(load))(context, "vkGetPipelineKeyKHR");
  vkReleaseCapturedPipelineDataKHR = (PFN_vkReleaseCapturedPipelineDataKHR)(this->*(load))(context, "vkReleaseCapturedPipelineDataKHR");
#endif /* defined(VK_KHR_pipeline_binary) */
#if defined(VK_KHR_pipeline_executable_properties)
  vkGetPipelineExecutableInternalRepresentationsKHR = (PFN_vkGetPipelineExecutableInternalRepresentationsKHR)(this->*(load))(context, "vkGetPipelineExecutableInternalRepresentationsKHR");
  vkGetPipelineExecutablePropertiesKHR = (PFN_vkGetPipelineExecutablePropertiesKHR)(this->*(load))(context, "vkGetPipelineExecutablePropertiesKHR");
  vkGetPipelineExecutableStatisticsKHR = (PFN_vkGetPipelineExecutableStatisticsKHR)(this->*(load))(context, "vkGetPipelineExecutableStatisticsKHR");
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
  vkWaitForPresentKHR = (PFN_vkWaitForPresentKHR)(this->*(load))(context, "vkWaitForPresentKHR");
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_present_wait2)
  vkWaitForPresent2KHR = (PFN_vkWaitForPresent2KHR)(this->*(load))(context, "vkWaitForPresent2KHR");
#endif /* defined(VK_KHR_present_wait2) */
#if defined(VK_KHR_push_descriptor)
  vkCmdPushDescriptorSetKHR = (PFN_vkCmdPushDescriptorSetKHR)(this->*(load))(context, "vkCmdPushDescriptorSetKHR");
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
  vkCmdTraceRaysIndirect2KHR = (PFN_vkCmdTraceRaysIndirect2KHR)(this->*(load))(context, "vkCmdTraceRaysIndirect2KHR");
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
  vkCmdSetRayTracingPipelineStackSizeKHR = (PFN_vkCmdSetRayTracingPipelineStackSizeKHR)(this->*(load))(context, "vkCmdSetRayTracingPipelineStackSizeKHR");
  vkCmdTraceRaysIndirectKHR = (PFN_vkCmdTraceRaysIndirectKHR)(this->*(load))(context, "vkCmdTraceRaysIndirectKHR");
  vkCmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR)(this->*(load))(context, "vkCmdTraceRaysKHR");
  vkCreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR)(this->*(load))(context, "vkCreateRayTracingPipelinesKHR");
  vkGetRayTracingCaptureReplayShaderGroupHandlesKHR = (PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR)(this->*(load))(context, "vkGetRayTracingCaptureReplayShaderGroupHandlesKHR");
  vkGetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR)(this->*(load))(context, "vkGetRayTracingShaderGroupHandlesKHR");
  vkGetRayTracingShaderGroupStackSizeKHR = (PFN_vkGetRayTracingShaderGroupStackSizeKHR)(this->*(load))(context, "vkGetRayTracingShaderGroupStackSizeKHR");
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
  vkCreateSamplerYcbcrConversionKHR = (PFN_vkCreateSamplerYcbcrConversionKHR)(this->*(load))(context, "vkCreateSamplerYcbcrConversionKHR");
  vkDestroySamplerYcbcrConversionKHR = (PFN_vkDestroySamplerYcbcrConversionKHR)(this->*(load))(context, "vkDestroySamplerYcbcrConversionKHR");
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
  vkGetSwapchainStatusKHR = (PFN_vkGetSwapchainStatusKHR)(this->*(load))(context, "vkGetSwapchainStatusKHR");
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
  vkAcquireNextImageKHR = (PFN_vkAcquireNextImageKHR)(this->*(load))(context, "vkAcquireNextImageKHR");
  vkCreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)(this->*(load))(context, "vkCreateSwapchainKHR");
  vkDestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)(this->*(load))(context, "vkDestroySwapchainKHR");
  vkGetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)(this->*(load))(context, "vkGetSwapchainImagesKHR");
  vkQueuePresentKHR = (PFN_vkQueuePresentKHR)(this->*(load))(context, "vkQueuePresentKHR");
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_swapchain_maintenance1)
  vkReleaseSwapchainImagesKHR = (PFN_vkReleaseSwapchainImagesKHR)(this->*(load))(context, "vkReleaseSwapchainImagesKHR");
#endif /* defined(VK_KHR_swapchain_maintenance1) */
#if defined(VK_KHR_synchronization2)
  vkCmdPipelineBarrier2KHR = (PFN_vkCmdPipelineBarrier2KHR)(this->*(load))(context, "vkCmdPipelineBarrier2KHR");
  vkCmdResetEvent2KHR = (PFN_vkCmdResetEvent2KHR)(this->*(load))(context, "vkCmdResetEvent2KHR");
  vkCmdSetEvent2KHR = (PFN_vkCmdSetEvent2KHR)(this->*(load))(context, "vkCmdSetEvent2KHR");
  vkCmdWaitEvents2KHR = (PFN_vkCmdWaitEvents2KHR)(this->*(load))(context, "vkCmdWaitEvents2KHR");
  vkCmdWriteTimestamp2KHR = (PFN_vkCmdWriteTimestamp2KHR)(this->*(load))(context, "vkCmdWriteTimestamp2KHR");
  vkQueueSubmit2KHR = (PFN_vkQueueSubmit2KHR)(this->*(load))(context, "vkQueueSubmit2KHR");
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_timeline_semaphore)
  vkGetSemaphoreCounterValueKHR = (PFN_vkGetSemaphoreCounterValueKHR)(this->*(load))(context, "vkGetSemaphoreCounterValueKHR");
  vkSignalSemaphoreKHR = (PFN_vkSignalSemaphoreKHR)(this->*(load))(context, "vkSignalSemaphoreKHR");
  vkWaitSemaphoresKHR = (PFN_vkWaitSemaphoresKHR)(this->*(load))(context, "vkWaitSemaphoresKHR");
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
  vkCmdDecodeVideoKHR = (PFN_vkCmdDecodeVideoKHR)(this->*(load))(context, "vkCmdDecodeVideoKHR");
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
  vkCmdEncodeVideoKHR = (PFN_vkCmdEncodeVideoKHR)(this->*(load))(context, "vkCmdEncodeVideoKHR");
  vkGetEncodedVideoSessionParametersKHR = (PFN_vkGetEncodedVideoSessionParametersKHR)(this->*(load))(context, "vkGetEncodedVideoSessionParametersKHR");
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
  vkBindVideoSessionMemoryKHR = (PFN_vkBindVideoSessionMemoryKHR)(this->*(load))(context, "vkBindVideoSessionMemoryKHR");
  vkCmdBeginVideoCodingKHR = (PFN_vkCmdBeginVideoCodingKHR)(this->*(load))(context, "vkCmdBeginVideoCodingKHR");
  vkCmdControlVideoCodingKHR = (PFN_vkCmdControlVideoCodingKHR)(this->*(load))(context, "vkCmdControlVideoCodingKHR");
  vkCmdEndVideoCodingKHR = (PFN_vkCmdEndVideoCodingKHR)(this->*(load))(context, "vkCmdEndVideoCodingKHR");
  vkCreateVideoSessionKHR = (PFN_vkCreateVideoSessionKHR)(this->*(load))(context, "vkCreateVideoSessionKHR");
  vkCreateVideoSessionParametersKHR = (PFN_vkCreateVideoSessionParametersKHR)(this->*(load))(context, "vkCreateVideoSessionParametersKHR");
  vkDestroyVideoSessionKHR = (PFN_vkDestroyVideoSessionKHR)(this->*(load))(context, "vkDestroyVideoSessionKHR");
  vkDestroyVideoSessionParametersKHR = (PFN_vkDestroyVideoSessionParametersKHR)(this->*(load))(context, "vkDestroyVideoSessionParametersKHR");
  vkGetVideoSessionMemoryRequirementsKHR = (PFN_vkGetVideoSessionMemoryRequirementsKHR)(this->*(load))(context, "vkGetVideoSessionMemoryRequirementsKHR");
  vkUpdateVideoSessionParametersKHR = (PFN_vkUpdateVideoSessionParametersKHR)(this->*(load))(context, "vkUpdateVideoSessionParametersKHR");
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
  vkCmdCuLaunchKernelNVX = (PFN_vkCmdCuLaunchKernelNVX)(this->*(load))(context, "vkCmdCuLaunchKernelNVX");
  vkCreateCuFunctionNVX = (PFN_vkCreateCuFunctionNVX)(this->*(load))(context, "vkCreateCuFunctionNVX");
  vkCreateCuModuleNVX = (PFN_vkCreateCuModuleNVX)(this->*(load))(context, "vkCreateCuModuleNVX");
  vkDestroyCuFunctionNVX = (PFN_vkDestroyCuFunctionNVX)(this->*(load))(context, "vkDestroyCuFunctionNVX");
  vkDestroyCuModuleNVX = (PFN_vkDestroyCuModuleNVX)(this->*(load))(context, "vkDestroyCuModuleNVX");
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
  vkGetImageViewAddressNVX = (PFN_vkGetImageViewAddressNVX)(this->*(load))(context, "vkGetImageViewAddressNVX");
  vkGetImageViewHandle64NVX = (PFN_vkGetImageViewHandle64NVX)(this->*(load))(context, "vkGetImageViewHandle64NVX");
  vkGetImageViewHandleNVX = (PFN_vkGetImageViewHandleNVX)(this->*(load))(context, "vkGetImageViewHandleNVX");
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_clip_space_w_scaling)
  vkCmdSetViewportWScalingNV = (PFN_vkCmdSetViewportWScalingNV)(this->*(load))(context, "vkCmdSetViewportWScalingNV");
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_cluster_acceleration_structure)
  vkCmdBuildClusterAccelerationStructureIndirectNV = (PFN_vkCmdBuildClusterAccelerationStructureIndirectNV)(this->*(load))(context, "vkCmdBuildClusterAccelerationStructureIndirectNV");
  vkGetClusterAccelerationStructureBuildSizesNV = (PFN_vkGetClusterAccelerationStructureBuildSizesNV)(this->*(load))(context, "vkGetClusterAccelerationStructureBuildSizesNV");
#endif /* defined(VK_NV_cluster_acceleration_structure) */
#if defined(VK_NV_cooperative_vector)
  vkCmdConvertCooperativeVectorMatrixNV = (PFN_vkCmdConvertCooperativeVectorMatrixNV)(this->*(load))(context, "vkCmdConvertCooperativeVectorMatrixNV");
  vkConvertCooperativeVectorMatrixNV = (PFN_vkConvertCooperativeVectorMatrixNV)(this->*(load))(context, "vkConvertCooperativeVectorMatrixNV");
#endif /* defined(VK_NV_cooperative_vector) */
#if defined(VK_NV_copy_memory_indirect)
  vkCmdCopyMemoryIndirectNV = (PFN_vkCmdCopyMemoryIndirectNV)(this->*(load))(context, "vkCmdCopyMemoryIndirectNV");
  vkCmdCopyMemoryToImageIndirectNV = (PFN_vkCmdCopyMemoryToImageIndirectNV)(this->*(load))(context, "vkCmdCopyMemoryToImageIndirectNV");
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_cuda_kernel_launch)
  vkCmdCudaLaunchKernelNV = (PFN_vkCmdCudaLaunchKernelNV)(this->*(load))(context, "vkCmdCudaLaunchKernelNV");
  vkCreateCudaFunctionNV = (PFN_vkCreateCudaFunctionNV)(this->*(load))(context, "vkCreateCudaFunctionNV");
  vkCreateCudaModuleNV = (PFN_vkCreateCudaModuleNV)(this->*(load))(context, "vkCreateCudaModuleNV");
  vkDestroyCudaFunctionNV = (PFN_vkDestroyCudaFunctionNV)(this->*(load))(context, "vkDestroyCudaFunctionNV");
  vkDestroyCudaModuleNV = (PFN_vkDestroyCudaModuleNV)(this->*(load))(context, "vkDestroyCudaModuleNV");
  vkGetCudaModuleCacheNV = (PFN_vkGetCudaModuleCacheNV)(this->*(load))(context, "vkGetCudaModuleCacheNV");
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
  vkCmdSetCheckpointNV = (PFN_vkCmdSetCheckpointNV)(this->*(load))(context, "vkCmdSetCheckpointNV");
  vkGetQueueCheckpointDataNV = (PFN_vkGetQueueCheckpointDataNV)(this->*(load))(context, "vkGetQueueCheckpointDataNV");
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
  vkGetQueueCheckpointData2NV = (PFN_vkGetQueueCheckpointData2NV)(this->*(load))(context, "vkGetQueueCheckpointData2NV");
#endif /* defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) */
#if defined(VK_NV_device_generated_commands)
  vkCmdBindPipelineShaderGroupNV = (PFN_vkCmdBindPipelineShaderGroupNV)(this->*(load))(context, "vkCmdBindPipelineShaderGroupNV");
  vkCmdExecuteGeneratedCommandsNV = (PFN_vkCmdExecuteGeneratedCommandsNV)(this->*(load))(context, "vkCmdExecuteGeneratedCommandsNV");
  vkCmdPreprocessGeneratedCommandsNV = (PFN_vkCmdPreprocessGeneratedCommandsNV)(this->*(load))(context, "vkCmdPreprocessGeneratedCommandsNV");
  vkCreateIndirectCommandsLayoutNV = (PFN_vkCreateIndirectCommandsLayoutNV)(this->*(load))(context, "vkCreateIndirectCommandsLayoutNV");
  vkDestroyIndirectCommandsLayoutNV = (PFN_vkDestroyIndirectCommandsLayoutNV)(this->*(load))(context, "vkDestroyIndirectCommandsLayoutNV");
  vkGetGeneratedCommandsMemoryRequirementsNV = (PFN_vkGetGeneratedCommandsMemoryRequirementsNV)(this->*(load))(context, "vkGetGeneratedCommandsMemoryRequirementsNV");
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
  vkCmdUpdatePipelineIndirectBufferNV = (PFN_vkCmdUpdatePipelineIndirectBufferNV)(this->*(load))(context, "vkCmdUpdatePipelineIndirectBufferNV");
  vkGetPipelineIndirectDeviceAddressNV = (PFN_vkGetPipelineIndirectDeviceAddressNV)(this->*(load))(context, "vkGetPipelineIndirectDeviceAddressNV");
  vkGetPipelineIndirectMemoryRequirementsNV = (PFN_vkGetPipelineIndirectMemoryRequirementsNV)(this->*(load))(context, "vkGetPipelineIndirectMemoryRequirementsNV");
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_compute_queue)
  vkCreateExternalComputeQueueNV = (PFN_vkCreateExternalComputeQueueNV)(this->*(load))(context, "vkCreateExternalComputeQueueNV");
  vkDestroyExternalComputeQueueNV = (PFN_vkDestroyExternalComputeQueueNV)(this->*(load))(context, "vkDestroyExternalComputeQueueNV");
  vkGetExternalComputeQueueDataNV = (PFN_vkGetExternalComputeQueueDataNV)(this->*(load))(context, "vkGetExternalComputeQueueDataNV");
#endif /* defined(VK_NV_external_compute_queue) */
#if defined(VK_NV_external_memory_rdma)
  vkGetMemoryRemoteAddressNV = (PFN_vkGetMemoryRemoteAddressNV)(this->*(load))(context, "vkGetMemoryRemoteAddressNV");
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
  vkGetMemoryWin32HandleNV = (PFN_vkGetMemoryWin32HandleNV)(this->*(load))(context, "vkGetMemoryWin32HandleNV");
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
  vkCmdSetFragmentShadingRateEnumNV = (PFN_vkCmdSetFragmentShadingRateEnumNV)(this->*(load))(context, "vkCmdSetFragmentShadingRateEnumNV");
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
  vkGetLatencyTimingsNV = (PFN_vkGetLatencyTimingsNV)(this->*(load))(context, "vkGetLatencyTimingsNV");
  vkLatencySleepNV = (PFN_vkLatencySleepNV)(this->*(load))(context, "vkLatencySleepNV");
  vkQueueNotifyOutOfBandNV = (PFN_vkQueueNotifyOutOfBandNV)(this->*(load))(context, "vkQueueNotifyOutOfBandNV");
  vkSetLatencyMarkerNV = (PFN_vkSetLatencyMarkerNV)(this->*(load))(context, "vkSetLatencyMarkerNV");
  vkSetLatencySleepModeNV = (PFN_vkSetLatencySleepModeNV)(this->*(load))(context, "vkSetLatencySleepModeNV");
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
  vkCmdDecompressMemoryIndirectCountNV = (PFN_vkCmdDecompressMemoryIndirectCountNV)(this->*(load))(context, "vkCmdDecompressMemoryIndirectCountNV");
  vkCmdDecompressMemoryNV = (PFN_vkCmdDecompressMemoryNV)(this->*(load))(context, "vkCmdDecompressMemoryNV");
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
  vkCmdDrawMeshTasksIndirectNV = (PFN_vkCmdDrawMeshTasksIndirectNV)(this->*(load))(context, "vkCmdDrawMeshTasksIndirectNV");
  vkCmdDrawMeshTasksNV = (PFN_vkCmdDrawMeshTasksNV)(this->*(load))(context, "vkCmdDrawMeshTasksNV");
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
  vkCmdDrawMeshTasksIndirectCountNV = (PFN_vkCmdDrawMeshTasksIndirectCountNV)(this->*(load))(context, "vkCmdDrawMeshTasksIndirectCountNV");
#endif /* defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) */
#if defined(VK_NV_optical_flow)
  vkBindOpticalFlowSessionImageNV = (PFN_vkBindOpticalFlowSessionImageNV)(this->*(load))(context, "vkBindOpticalFlowSessionImageNV");
  vkCmdOpticalFlowExecuteNV = (PFN_vkCmdOpticalFlowExecuteNV)(this->*(load))(context, "vkCmdOpticalFlowExecuteNV");
  vkCreateOpticalFlowSessionNV = (PFN_vkCreateOpticalFlowSessionNV)(this->*(load))(context, "vkCreateOpticalFlowSessionNV");
  vkDestroyOpticalFlowSessionNV = (PFN_vkDestroyOpticalFlowSessionNV)(this->*(load))(context, "vkDestroyOpticalFlowSessionNV");
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_partitioned_acceleration_structure)
  vkCmdBuildPartitionedAccelerationStructuresNV = (PFN_vkCmdBuildPartitionedAccelerationStructuresNV)(this->*(load))(context, "vkCmdBuildPartitionedAccelerationStructuresNV");
  vkGetPartitionedAccelerationStructuresBuildSizesNV = (PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV)(this->*(load))(context, "vkGetPartitionedAccelerationStructuresBuildSizesNV");
#endif /* defined(VK_NV_partitioned_acceleration_structure) */
#if defined(VK_NV_ray_tracing)
  vkBindAccelerationStructureMemoryNV = (PFN_vkBindAccelerationStructureMemoryNV)(this->*(load))(context, "vkBindAccelerationStructureMemoryNV");
  vkCmdBuildAccelerationStructureNV = (PFN_vkCmdBuildAccelerationStructureNV)(this->*(load))(context, "vkCmdBuildAccelerationStructureNV");
  vkCmdCopyAccelerationStructureNV = (PFN_vkCmdCopyAccelerationStructureNV)(this->*(load))(context, "vkCmdCopyAccelerationStructureNV");
  vkCmdTraceRaysNV = (PFN_vkCmdTraceRaysNV)(this->*(load))(context, "vkCmdTraceRaysNV");
  vkCmdWriteAccelerationStructuresPropertiesNV = (PFN_vkCmdWriteAccelerationStructuresPropertiesNV)(this->*(load))(context, "vkCmdWriteAccelerationStructuresPropertiesNV");
  vkCompileDeferredNV = (PFN_vkCompileDeferredNV)(this->*(load))(context, "vkCompileDeferredNV");
  vkCreateAccelerationStructureNV = (PFN_vkCreateAccelerationStructureNV)(this->*(load))(context, "vkCreateAccelerationStructureNV");
  vkCreateRayTracingPipelinesNV = (PFN_vkCreateRayTracingPipelinesNV)(this->*(load))(context, "vkCreateRayTracingPipelinesNV");
  vkDestroyAccelerationStructureNV = (PFN_vkDestroyAccelerationStructureNV)(this->*(load))(context, "vkDestroyAccelerationStructureNV");
  vkGetAccelerationStructureHandleNV = (PFN_vkGetAccelerationStructureHandleNV)(this->*(load))(context, "vkGetAccelerationStructureHandleNV");
  vkGetAccelerationStructureMemoryRequirementsNV = (PFN_vkGetAccelerationStructureMemoryRequirementsNV)(this->*(load))(context, "vkGetAccelerationStructureMemoryRequirementsNV");
  vkGetRayTracingShaderGroupHandlesNV = (PFN_vkGetRayTracingShaderGroupHandlesNV)(this->*(load))(context, "vkGetRayTracingShaderGroupHandlesNV");
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
  vkCmdSetExclusiveScissorEnableNV = (PFN_vkCmdSetExclusiveScissorEnableNV)(this->*(load))(context, "vkCmdSetExclusiveScissorEnableNV");
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
  vkCmdSetExclusiveScissorNV = (PFN_vkCmdSetExclusiveScissorNV)(this->*(load))(context, "vkCmdSetExclusiveScissorNV");
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
  vkCmdBindShadingRateImageNV = (PFN_vkCmdBindShadingRateImageNV)(this->*(load))(context, "vkCmdBindShadingRateImageNV");
  vkCmdSetCoarseSampleOrderNV = (PFN_vkCmdSetCoarseSampleOrderNV)(this->*(load))(context, "vkCmdSetCoarseSampleOrderNV");
  vkCmdSetViewportShadingRatePaletteNV = (PFN_vkCmdSetViewportShadingRatePaletteNV)(this->*(load))(context, "vkCmdSetViewportShadingRatePaletteNV");
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_OHOS_external_memory)
  vkGetMemoryNativeBufferOHOS = (PFN_vkGetMemoryNativeBufferOHOS)(this->*(load))(context, "vkGetMemoryNativeBufferOHOS");
  vkGetNativeBufferPropertiesOHOS = (PFN_vkGetNativeBufferPropertiesOHOS)(this->*(load))(context, "vkGetNativeBufferPropertiesOHOS");
#endif /* defined(VK_OHOS_external_memory) */
#if defined(VK_OHOS_native_buffer)
  vkAcquireImageOHOS = (PFN_vkAcquireImageOHOS)(this->*(load))(context, "vkAcquireImageOHOS");
  vkGetSwapchainGrallocUsageOHOS = (PFN_vkGetSwapchainGrallocUsageOHOS)(this->*(load))(context, "vkGetSwapchainGrallocUsageOHOS");
  vkQueueSignalReleaseImageOHOS = (PFN_vkQueueSignalReleaseImageOHOS)(this->*(load))(context, "vkQueueSignalReleaseImageOHOS");
#endif /* defined(VK_OHOS_native_buffer) */
#if defined(VK_QCOM_tile_memory_heap)
  vkCmdBindTileMemoryQCOM = (PFN_vkCmdBindTileMemoryQCOM)(this->*(load))(context, "vkCmdBindTileMemoryQCOM");
#endif /* defined(VK_QCOM_tile_memory_heap) */
#if defined(VK_QCOM_tile_properties)
  vkGetDynamicRenderingTilePropertiesQCOM = (PFN_vkGetDynamicRenderingTilePropertiesQCOM)(this->*(load))(context, "vkGetDynamicRenderingTilePropertiesQCOM");
  vkGetFramebufferTilePropertiesQCOM = (PFN_vkGetFramebufferTilePropertiesQCOM)(this->*(load))(context, "vkGetFramebufferTilePropertiesQCOM");
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QCOM_tile_shading)
  vkCmdBeginPerTileExecutionQCOM = (PFN_vkCmdBeginPerTileExecutionQCOM)(this->*(load))(context, "vkCmdBeginPerTileExecutionQCOM");
  vkCmdDispatchTileQCOM = (PFN_vkCmdDispatchTileQCOM)(this->*(load))(context, "vkCmdDispatchTileQCOM");
  vkCmdEndPerTileExecutionQCOM = (PFN_vkCmdEndPerTileExecutionQCOM)(this->*(load))(context, "vkCmdEndPerTileExecutionQCOM");
#endif /* defined(VK_QCOM_tile_shading) */
#if defined(VK_QNX_external_memory_screen_buffer)
  vkGetScreenBufferPropertiesQNX = (PFN_vkGetScreenBufferPropertiesQNX)(this->*(load))(context, "vkGetScreenBufferPropertiesQNX");
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
  vkGetDescriptorSetHostMappingVALVE = (PFN_vkGetDescriptorSetHostMappingVALVE)(this->*(load))(context, "vkGetDescriptorSetHostMappingVALVE");
  vkGetDescriptorSetLayoutHostMappingInfoVALVE = (PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE)(this->*(load))(context, "vkGetDescriptorSetLayoutHostMappingInfoVALVE");
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control))
  vkCmdSetDepthClampRangeEXT = (PFN_vkCmdSetDepthClampRangeEXT)(this->*(load))(context, "vkCmdSetDepthClampRangeEXT");
#endif /* (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
  vkCmdBindVertexBuffers2EXT = (PFN_vkCmdBindVertexBuffers2EXT)(this->*(load))(context, "vkCmdBindVertexBuffers2EXT");
  vkCmdSetCullModeEXT = (PFN_vkCmdSetCullModeEXT)(this->*(load))(context, "vkCmdSetCullModeEXT");
  vkCmdSetDepthBoundsTestEnableEXT = (PFN_vkCmdSetDepthBoundsTestEnableEXT)(this->*(load))(context, "vkCmdSetDepthBoundsTestEnableEXT");
  vkCmdSetDepthCompareOpEXT = (PFN_vkCmdSetDepthCompareOpEXT)(this->*(load))(context, "vkCmdSetDepthCompareOpEXT");
  vkCmdSetDepthTestEnableEXT = (PFN_vkCmdSetDepthTestEnableEXT)(this->*(load))(context, "vkCmdSetDepthTestEnableEXT");
  vkCmdSetDepthWriteEnableEXT = (PFN_vkCmdSetDepthWriteEnableEXT)(this->*(load))(context, "vkCmdSetDepthWriteEnableEXT");
  vkCmdSetFrontFaceEXT = (PFN_vkCmdSetFrontFaceEXT)(this->*(load))(context, "vkCmdSetFrontFaceEXT");
  vkCmdSetPrimitiveTopologyEXT = (PFN_vkCmdSetPrimitiveTopologyEXT)(this->*(load))(context, "vkCmdSetPrimitiveTopologyEXT");
  vkCmdSetScissorWithCountEXT = (PFN_vkCmdSetScissorWithCountEXT)(this->*(load))(context, "vkCmdSetScissorWithCountEXT");
  vkCmdSetStencilOpEXT = (PFN_vkCmdSetStencilOpEXT)(this->*(load))(context, "vkCmdSetStencilOpEXT");
  vkCmdSetStencilTestEnableEXT = (PFN_vkCmdSetStencilTestEnableEXT)(this->*(load))(context, "vkCmdSetStencilTestEnableEXT");
  vkCmdSetViewportWithCountEXT = (PFN_vkCmdSetViewportWithCountEXT)(this->*(load))(context, "vkCmdSetViewportWithCountEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
  vkCmdSetDepthBiasEnableEXT = (PFN_vkCmdSetDepthBiasEnableEXT)(this->*(load))(context, "vkCmdSetDepthBiasEnableEXT");
  vkCmdSetLogicOpEXT = (PFN_vkCmdSetLogicOpEXT)(this->*(load))(context, "vkCmdSetLogicOpEXT");
  vkCmdSetPatchControlPointsEXT = (PFN_vkCmdSetPatchControlPointsEXT)(this->*(load))(context, "vkCmdSetPatchControlPointsEXT");
  vkCmdSetPrimitiveRestartEnableEXT = (PFN_vkCmdSetPrimitiveRestartEnableEXT)(this->*(load))(context, "vkCmdSetPrimitiveRestartEnableEXT");
  vkCmdSetRasterizerDiscardEnableEXT = (PFN_vkCmdSetRasterizerDiscardEnableEXT)(this->*(load))(context, "vkCmdSetRasterizerDiscardEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
  vkCmdSetAlphaToCoverageEnableEXT = (PFN_vkCmdSetAlphaToCoverageEnableEXT)(this->*(load))(context, "vkCmdSetAlphaToCoverageEnableEXT");
  vkCmdSetAlphaToOneEnableEXT = (PFN_vkCmdSetAlphaToOneEnableEXT)(this->*(load))(context, "vkCmdSetAlphaToOneEnableEXT");
  vkCmdSetColorBlendEnableEXT = (PFN_vkCmdSetColorBlendEnableEXT)(this->*(load))(context, "vkCmdSetColorBlendEnableEXT");
  vkCmdSetColorBlendEquationEXT = (PFN_vkCmdSetColorBlendEquationEXT)(this->*(load))(context, "vkCmdSetColorBlendEquationEXT");
  vkCmdSetColorWriteMaskEXT = (PFN_vkCmdSetColorWriteMaskEXT)(this->*(load))(context, "vkCmdSetColorWriteMaskEXT");
  vkCmdSetDepthClampEnableEXT = (PFN_vkCmdSetDepthClampEnableEXT)(this->*(load))(context, "vkCmdSetDepthClampEnableEXT");
  vkCmdSetLogicOpEnableEXT = (PFN_vkCmdSetLogicOpEnableEXT)(this->*(load))(context, "vkCmdSetLogicOpEnableEXT");
  vkCmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)(this->*(load))(context, "vkCmdSetPolygonModeEXT");
  vkCmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT)(this->*(load))(context, "vkCmdSetRasterizationSamplesEXT");
  vkCmdSetSampleMaskEXT = (PFN_vkCmdSetSampleMaskEXT)(this->*(load))(context, "vkCmdSetSampleMaskEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
  vkCmdSetTessellationDomainOriginEXT = (PFN_vkCmdSetTessellationDomainOriginEXT)(this->*(load))(context, "vkCmdSetTessellationDomainOriginEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
  vkCmdSetRasterizationStreamEXT = (PFN_vkCmdSetRasterizationStreamEXT)(this->*(load))(context, "vkCmdSetRasterizationStreamEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
  vkCmdSetConservativeRasterizationModeEXT = (PFN_vkCmdSetConservativeRasterizationModeEXT)(this->*(load))(context, "vkCmdSetConservativeRasterizationModeEXT");
  vkCmdSetExtraPrimitiveOverestimationSizeEXT = (PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT)(this->*(load))(context, "vkCmdSetExtraPrimitiveOverestimationSizeEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
  vkCmdSetDepthClipEnableEXT = (PFN_vkCmdSetDepthClipEnableEXT)(this->*(load))(context, "vkCmdSetDepthClipEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
  vkCmdSetSampleLocationsEnableEXT = (PFN_vkCmdSetSampleLocationsEnableEXT)(this->*(load))(context, "vkCmdSetSampleLocationsEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
  vkCmdSetColorBlendAdvancedEXT = (PFN_vkCmdSetColorBlendAdvancedEXT)(this->*(load))(context, "vkCmdSetColorBlendAdvancedEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
  vkCmdSetProvokingVertexModeEXT = (PFN_vkCmdSetProvokingVertexModeEXT)(this->*(load))(context, "vkCmdSetProvokingVertexModeEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
  vkCmdSetLineRasterizationModeEXT = (PFN_vkCmdSetLineRasterizationModeEXT)(this->*(load))(context, "vkCmdSetLineRasterizationModeEXT");
  vkCmdSetLineStippleEnableEXT = (PFN_vkCmdSetLineStippleEnableEXT)(this->*(load))(context, "vkCmdSetLineStippleEnableEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
  vkCmdSetDepthClipNegativeOneToOneEXT = (PFN_vkCmdSetDepthClipNegativeOneToOneEXT)(this->*(load))(context, "vkCmdSetDepthClipNegativeOneToOneEXT");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
  vkCmdSetViewportWScalingEnableNV = (PFN_vkCmdSetViewportWScalingEnableNV)(this->*(load))(context, "vkCmdSetViewportWScalingEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
  vkCmdSetViewportSwizzleNV = (PFN_vkCmdSetViewportSwizzleNV)(this->*(load))(context, "vkCmdSetViewportSwizzleNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
  vkCmdSetCoverageToColorEnableNV = (PFN_vkCmdSetCoverageToColorEnableNV)(this->*(load))(context, "vkCmdSetCoverageToColorEnableNV");
  vkCmdSetCoverageToColorLocationNV = (PFN_vkCmdSetCoverageToColorLocationNV)(this->*(load))(context, "vkCmdSetCoverageToColorLocationNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
  vkCmdSetCoverageModulationModeNV = (PFN_vkCmdSetCoverageModulationModeNV)(this->*(load))(context, "vkCmdSetCoverageModulationModeNV");
  vkCmdSetCoverageModulationTableEnableNV = (PFN_vkCmdSetCoverageModulationTableEnableNV)(this->*(load))(context, "vkCmdSetCoverageModulationTableEnableNV");
  vkCmdSetCoverageModulationTableNV = (PFN_vkCmdSetCoverageModulationTableNV)(this->*(load))(context, "vkCmdSetCoverageModulationTableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
  vkCmdSetShadingRateImageEnableNV = (PFN_vkCmdSetShadingRateImageEnableNV)(this->*(load))(context, "vkCmdSetShadingRateImageEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
  vkCmdSetRepresentativeFragmentTestEnableNV = (PFN_vkCmdSetRepresentativeFragmentTestEnableNV)(this->*(load))(context, "vkCmdSetRepresentativeFragmentTestEnableNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
  vkCmdSetCoverageReductionModeNV = (PFN_vkCmdSetCoverageReductionModeNV)(this->*(load))(context, "vkCmdSetCoverageReductionModeNV");
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
  vkGetImageSubresourceLayout2EXT = (PFN_vkGetImageSubresourceLayout2EXT)(this->*(load))(context, "vkGetImageSubresourceLayout2EXT");
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
  vkCmdSetVertexInputEXT = (PFN_vkCmdSetVertexInputEXT)(this->*(load))(context, "vkCmdSetVertexInputEXT");
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template)))
  vkCmdPushDescriptorSetWithTemplateKHR = (PFN_vkCmdPushDescriptorSetWithTemplateKHR)(this->*(load))(context, "vkCmdPushDescriptorSetWithTemplateKHR");
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template))) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
  vkGetDeviceGroupPresentCapabilitiesKHR = (PFN_vkGetDeviceGroupPresentCapabilitiesKHR)(this->*(load))(context, "vkGetDeviceGroupPresentCapabilitiesKHR");
  vkGetDeviceGroupSurfacePresentModesKHR = (PFN_vkGetDeviceGroupSurfacePresentModesKHR)(this->*(load))(context, "vkGetDeviceGroupSurfacePresentModesKHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
  vkAcquireNextImage2KHR = (PFN_vkAcquireNextImage2KHR)(this->*(load))(context, "vkAcquireNextImage2KHR");
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
  /* VOLK_CPP_GENERATE_LOAD_DEVICE */
}

/* clang-format on */
