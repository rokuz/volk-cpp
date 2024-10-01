/**
 * volk-cpp by rokuz (https://github.com/rokuz/volk-cpp)
 *
 * C++20 port of volk library (https://github.com/zeux/volk).
 */

/**
 * volk
 *
 * Copyright (C) 2018-2023, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at https://github.com/zeux/volk
 *
 * This library is distributed under the MIT License. See notice at the end of this file.
 */
/* clang-format off */
#pragma once

#if defined(VULKAN_H_) && !defined(VK_NO_PROTOTYPES)
  #error To use volk-cpp, you need to define VK_NO_PROTOTYPES before including vulkan.h
#endif

/* VOLK_CPP_GENERATE_VERSION_DEFINE */
#define VOLK_CPP_HEADER_VERSION 296
/* VOLK_CPP_GENERATE_VERSION_DEFINE */

#ifndef VK_NO_PROTOTYPES
  #define VK_NO_PROTOTYPES
#endif

#ifndef VULKAN_H_
  #ifdef VOLK_CPP_VULKAN_H_PATH
    #include VOLK_CPP_VULKAN_H_PATH
  #elif defined(VK_USE_PLATFORM_WIN32_KHR)
    #include <vulkan/vk_platform.h>
    #include <vulkan/vulkan_core.h>

    /* When VK_USE_PLATFORM_WIN32_KHR is defined, instead of including vulkan.h directly, we include individual parts of the SDK
     * This is necessary to avoid including <windows.h> which is very heavy - it takes 200ms to parse without WIN32_LEAN_AND_MEAN
     * and 100ms to parse with it. vulkan_win32.h only needs a few symbols that are easy to redefine ourselves.
     */
    typedef unsigned long DWORD;
    typedef const wchar_t* LPCWSTR;
    typedef void* HANDLE;
    typedef struct HINSTANCE__* HINSTANCE;
    typedef struct HWND__* HWND;
    typedef struct HMONITOR__* HMONITOR;
    typedef struct _SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;

    #include <vulkan/vulkan_win32.h>

    #ifdef VK_ENABLE_BETA_EXTENSIONS
      #include <vulkan/vulkan_beta.h>
    #endif
  #elif defined(__APPLE__)
    #include <vulkan/vulkan.h>
    #include <vulkan/vulkan_metal.h>
  #else
    #include <vulkan/vulkan.h>
  #endif
#endif

/* Disable several extensions on earlier SDKs because later SDKs introduce a backwards incompatible change to function signatures */
#if VK_HEADER_VERSION < 140
  #undef VK_NVX_image_view_handle
#endif
#if VK_HEADER_VERSION < 184
  #undef VK_HUAWEI_subpass_shading
#endif

#include <cstdint>

class Volk final {
public:
  /**
   * The handler function pointer will be asked to load Vulkan symbols which require no instance
   * (such as vkCreateInstance, vkEnumerateInstance* and vkEnumerateInstanceVersion if available).
   */
  explicit Volk(PFN_vkGetInstanceProcAddr handler = nullptr) noexcept;
  ~Volk() noexcept;
  Volk(const Volk&) = delete;
  Volk& operator=(const Volk&) = delete;
  Volk(Volk&&) = default;
  Volk& operator=(Volk&&) = default;
  
  /**
   * Get version of Vulkan Header with which this class was compiled.
   *
   * Returns VK_SUCCESS on success and VK_ERROR_INITIALIZATION_FAILED otherwise.
   */
  [[nodiscard]] uint32_t getVolkHeaderVersion() noexcept;
  
  /**
   * Get status of Volk instance initialization.
   *
   * Returns VK_SUCCESS on success and VK_ERROR_INITIALIZATION_FAILED otherwise.
   */
  [[nodiscard]] VkResult getStatus() noexcept;
  
  /**
   * Get Vulkan instance version supported by the Vulkan loader, or 0 if Vulkan isn't supported
   *
   * Returns 0 if Volk instance isn't valid.
   */
  [[nodiscard]] uint32_t getInstanceVersion() noexcept;

  /**
   * Load function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
   */
  void loadInstance(VkInstance instance) noexcept;

  /**
   * Load function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
   * Skips loading device-based function pointers, requires usage of loadDevice afterwards.
   */
  void loadInstanceOnly(VkInstance instance) noexcept;

  /**
   * Load function pointers using application-created VkDevice; call this function after creating the Vulkan device.
   *
   * Note: this is not suitable for applications that want to use multiple VkDevice objects concurrently.
   */
  void loadDevice(VkDevice device) noexcept;

  /**
   * Return last VkInstance for which function pointers have been loaded via loadInstance(),
   * or VK_NULL_HANDLE if loadInstance() has not been called.
   */
  [[nodiscard]] VkInstance getLoadedInstance() noexcept;

  /**
   * Return last VkDevice for which function pointers have been loaded via loadDevice(),
   * or VK_NULL_HANDLE if loadDevice() has not been called.
   */
  [[nodiscard]] VkDevice getLoadedDevice() noexcept;
  
#define ALIGNMENT(index) \
  private: \
    PFN_vkVoidFunction __alignment##index = nullptr; \
  public:
 
#define ALIGNMENT_ARRAY(index, size) \
  private: \
    PFN_vkVoidFunction __alignment##index[size] = {}; \
  public:

  /* VOLK_CPP_GENERATE_CLASS_FUNCTION_POINTERS */
#if defined(VK_VERSION_1_0)
  PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers = nullptr;
  PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets = nullptr;
  PFN_vkAllocateMemory vkAllocateMemory = nullptr;
  PFN_vkBeginCommandBuffer vkBeginCommandBuffer = nullptr;
  PFN_vkBindBufferMemory vkBindBufferMemory = nullptr;
  PFN_vkBindImageMemory vkBindImageMemory = nullptr;
  PFN_vkCmdBeginQuery vkCmdBeginQuery = nullptr;
  PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass = nullptr;
  PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets = nullptr;
  PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer = nullptr;
  PFN_vkCmdBindPipeline vkCmdBindPipeline = nullptr;
  PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers = nullptr;
  PFN_vkCmdBlitImage vkCmdBlitImage = nullptr;
  PFN_vkCmdClearAttachments vkCmdClearAttachments = nullptr;
  PFN_vkCmdClearColorImage vkCmdClearColorImage = nullptr;
  PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage = nullptr;
  PFN_vkCmdCopyBuffer vkCmdCopyBuffer = nullptr;
  PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage = nullptr;
  PFN_vkCmdCopyImage vkCmdCopyImage = nullptr;
  PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer = nullptr;
  PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults = nullptr;
  PFN_vkCmdDispatch vkCmdDispatch = nullptr;
  PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect = nullptr;
  PFN_vkCmdDraw vkCmdDraw = nullptr;
  PFN_vkCmdDrawIndexed vkCmdDrawIndexed = nullptr;
  PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect = nullptr;
  PFN_vkCmdDrawIndirect vkCmdDrawIndirect = nullptr;
  PFN_vkCmdEndQuery vkCmdEndQuery = nullptr;
  PFN_vkCmdEndRenderPass vkCmdEndRenderPass = nullptr;
  PFN_vkCmdExecuteCommands vkCmdExecuteCommands = nullptr;
  PFN_vkCmdFillBuffer vkCmdFillBuffer = nullptr;
  PFN_vkCmdNextSubpass vkCmdNextSubpass = nullptr;
  PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier = nullptr;
  PFN_vkCmdPushConstants vkCmdPushConstants = nullptr;
  PFN_vkCmdResetEvent vkCmdResetEvent = nullptr;
  PFN_vkCmdResetQueryPool vkCmdResetQueryPool = nullptr;
  PFN_vkCmdResolveImage vkCmdResolveImage = nullptr;
  PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants = nullptr;
  PFN_vkCmdSetDepthBias vkCmdSetDepthBias = nullptr;
  PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds = nullptr;
  PFN_vkCmdSetEvent vkCmdSetEvent = nullptr;
  PFN_vkCmdSetLineWidth vkCmdSetLineWidth = nullptr;
  PFN_vkCmdSetScissor vkCmdSetScissor = nullptr;
  PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask = nullptr;
  PFN_vkCmdSetStencilReference vkCmdSetStencilReference = nullptr;
  PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask = nullptr;
  PFN_vkCmdSetViewport vkCmdSetViewport = nullptr;
  PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer = nullptr;
  PFN_vkCmdWaitEvents vkCmdWaitEvents = nullptr;
  PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp = nullptr;
  PFN_vkCreateBuffer vkCreateBuffer = nullptr;
  PFN_vkCreateBufferView vkCreateBufferView = nullptr;
  PFN_vkCreateCommandPool vkCreateCommandPool = nullptr;
  PFN_vkCreateComputePipelines vkCreateComputePipelines = nullptr;
  PFN_vkCreateDescriptorPool vkCreateDescriptorPool = nullptr;
  PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout = nullptr;
  PFN_vkCreateDevice vkCreateDevice = nullptr;
  PFN_vkCreateEvent vkCreateEvent = nullptr;
  PFN_vkCreateFence vkCreateFence = nullptr;
  PFN_vkCreateFramebuffer vkCreateFramebuffer = nullptr;
  PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines = nullptr;
  PFN_vkCreateImage vkCreateImage = nullptr;
  PFN_vkCreateImageView vkCreateImageView = nullptr;
  PFN_vkCreateInstance vkCreateInstance = nullptr;
  PFN_vkCreatePipelineCache vkCreatePipelineCache = nullptr;
  PFN_vkCreatePipelineLayout vkCreatePipelineLayout = nullptr;
  PFN_vkCreateQueryPool vkCreateQueryPool = nullptr;
  PFN_vkCreateRenderPass vkCreateRenderPass = nullptr;
  PFN_vkCreateSampler vkCreateSampler = nullptr;
  PFN_vkCreateSemaphore vkCreateSemaphore = nullptr;
  PFN_vkCreateShaderModule vkCreateShaderModule = nullptr;
  PFN_vkDestroyBuffer vkDestroyBuffer = nullptr;
  PFN_vkDestroyBufferView vkDestroyBufferView = nullptr;
  PFN_vkDestroyCommandPool vkDestroyCommandPool = nullptr;
  PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool = nullptr;
  PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout = nullptr;
  PFN_vkDestroyDevice vkDestroyDevice = nullptr;
  PFN_vkDestroyEvent vkDestroyEvent = nullptr;
  PFN_vkDestroyFence vkDestroyFence = nullptr;
  PFN_vkDestroyFramebuffer vkDestroyFramebuffer = nullptr;
  PFN_vkDestroyImage vkDestroyImage = nullptr;
  PFN_vkDestroyImageView vkDestroyImageView = nullptr;
  PFN_vkDestroyInstance vkDestroyInstance = nullptr;
  PFN_vkDestroyPipeline vkDestroyPipeline = nullptr;
  PFN_vkDestroyPipelineCache vkDestroyPipelineCache = nullptr;
  PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout = nullptr;
  PFN_vkDestroyQueryPool vkDestroyQueryPool = nullptr;
  PFN_vkDestroyRenderPass vkDestroyRenderPass = nullptr;
  PFN_vkDestroySampler vkDestroySampler = nullptr;
  PFN_vkDestroySemaphore vkDestroySemaphore = nullptr;
  PFN_vkDestroyShaderModule vkDestroyShaderModule = nullptr;
  PFN_vkDeviceWaitIdle vkDeviceWaitIdle = nullptr;
  PFN_vkEndCommandBuffer vkEndCommandBuffer = nullptr;
  PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties = nullptr;
  PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties = nullptr;
  PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties = nullptr;
  PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties = nullptr;
  PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = nullptr;
  PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges = nullptr;
  PFN_vkFreeCommandBuffers vkFreeCommandBuffers = nullptr;
  PFN_vkFreeDescriptorSets vkFreeDescriptorSets = nullptr;
  PFN_vkFreeMemory vkFreeMemory = nullptr;
  PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements = nullptr;
  PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment = nullptr;
  PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr = nullptr;
  PFN_vkGetDeviceQueue vkGetDeviceQueue = nullptr;
  PFN_vkGetEventStatus vkGetEventStatus = nullptr;
  PFN_vkGetFenceStatus vkGetFenceStatus = nullptr;
  PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements = nullptr;
  PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements = nullptr;
  PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout = nullptr;
  PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = nullptr;
  PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = nullptr;
  PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties = nullptr;
  PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties = nullptr;
  PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = nullptr;
  PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = nullptr;
  PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = nullptr;
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties = nullptr;
  PFN_vkGetPipelineCacheData vkGetPipelineCacheData = nullptr;
  PFN_vkGetQueryPoolResults vkGetQueryPoolResults = nullptr;
  PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity = nullptr;
  PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges = nullptr;
  PFN_vkMapMemory vkMapMemory = nullptr;
  PFN_vkMergePipelineCaches vkMergePipelineCaches = nullptr;
  PFN_vkQueueBindSparse vkQueueBindSparse = nullptr;
  PFN_vkQueueSubmit vkQueueSubmit = nullptr;
  PFN_vkQueueWaitIdle vkQueueWaitIdle = nullptr;
  PFN_vkResetCommandBuffer vkResetCommandBuffer = nullptr;
  PFN_vkResetCommandPool vkResetCommandPool = nullptr;
  PFN_vkResetDescriptorPool vkResetDescriptorPool = nullptr;
  PFN_vkResetEvent vkResetEvent = nullptr;
  PFN_vkResetFences vkResetFences = nullptr;
  PFN_vkSetEvent vkSetEvent = nullptr;
  PFN_vkUnmapMemory vkUnmapMemory = nullptr;
  PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets = nullptr;
  PFN_vkWaitForFences vkWaitForFences = nullptr;
#else 
  ALIGNMENT_ARRAY(0, 137);
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
  PFN_vkBindBufferMemory2 vkBindBufferMemory2 = nullptr;
  PFN_vkBindImageMemory2 vkBindImageMemory2 = nullptr;
  PFN_vkCmdDispatchBase vkCmdDispatchBase = nullptr;
  PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask = nullptr;
  PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate = nullptr;
  PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion = nullptr;
  PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate = nullptr;
  PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion = nullptr;
  PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion = nullptr;
  PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups = nullptr;
  PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2 = nullptr;
  PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport = nullptr;
  PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures = nullptr;
  PFN_vkGetDeviceQueue2 vkGetDeviceQueue2 = nullptr;
  PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2 = nullptr;
  PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2 = nullptr;
  PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties = nullptr;
  PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties = nullptr;
  PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties = nullptr;
  PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2 = nullptr;
  PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2 = nullptr;
  PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2 = nullptr;
  PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2 = nullptr;
  PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2 = nullptr;
  PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2 = nullptr;
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2 = nullptr;
  PFN_vkTrimCommandPool vkTrimCommandPool = nullptr;
  PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate = nullptr;
#else 
  ALIGNMENT_ARRAY(1, 28);
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
  PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2 = nullptr;
  PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount = nullptr;
  PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount = nullptr;
  PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2 = nullptr;
  PFN_vkCmdNextSubpass2 vkCmdNextSubpass2 = nullptr;
  PFN_vkCreateRenderPass2 vkCreateRenderPass2 = nullptr;
  PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress = nullptr;
  PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress = nullptr;
  PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress = nullptr;
  PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue = nullptr;
  PFN_vkResetQueryPool vkResetQueryPool = nullptr;
  PFN_vkSignalSemaphore vkSignalSemaphore = nullptr;
  PFN_vkWaitSemaphores vkWaitSemaphores = nullptr;
#else 
  ALIGNMENT_ARRAY(2, 13);
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
  PFN_vkCmdBeginRendering vkCmdBeginRendering = nullptr;
  PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2 = nullptr;
  PFN_vkCmdBlitImage2 vkCmdBlitImage2 = nullptr;
  PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2 = nullptr;
  PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2 = nullptr;
  PFN_vkCmdCopyImage2 vkCmdCopyImage2 = nullptr;
  PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2 = nullptr;
  PFN_vkCmdEndRendering vkCmdEndRendering = nullptr;
  PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2 = nullptr;
  PFN_vkCmdResetEvent2 vkCmdResetEvent2 = nullptr;
  PFN_vkCmdResolveImage2 vkCmdResolveImage2 = nullptr;
  PFN_vkCmdSetCullMode vkCmdSetCullMode = nullptr;
  PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable = nullptr;
  PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable = nullptr;
  PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp = nullptr;
  PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable = nullptr;
  PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable = nullptr;
  PFN_vkCmdSetEvent2 vkCmdSetEvent2 = nullptr;
  PFN_vkCmdSetFrontFace vkCmdSetFrontFace = nullptr;
  PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable = nullptr;
  PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology = nullptr;
  PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable = nullptr;
  PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount = nullptr;
  PFN_vkCmdSetStencilOp vkCmdSetStencilOp = nullptr;
  PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable = nullptr;
  PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount = nullptr;
  PFN_vkCmdWaitEvents2 vkCmdWaitEvents2 = nullptr;
  PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2 = nullptr;
  PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot = nullptr;
  PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot = nullptr;
  PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements = nullptr;
  PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements = nullptr;
  PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements = nullptr;
  PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties = nullptr;
  PFN_vkGetPrivateData vkGetPrivateData = nullptr;
  PFN_vkQueueSubmit2 vkQueueSubmit2 = nullptr;
  PFN_vkSetPrivateData vkSetPrivateData = nullptr;
#else 
  ALIGNMENT_ARRAY(3, 37);
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_AMDX_shader_enqueue)
  PFN_vkCmdDispatchGraphAMDX vkCmdDispatchGraphAMDX = nullptr;
  PFN_vkCmdDispatchGraphIndirectAMDX vkCmdDispatchGraphIndirectAMDX = nullptr;
  PFN_vkCmdDispatchGraphIndirectCountAMDX vkCmdDispatchGraphIndirectCountAMDX = nullptr;
  PFN_vkCmdInitializeGraphScratchMemoryAMDX vkCmdInitializeGraphScratchMemoryAMDX = nullptr;
  PFN_vkCreateExecutionGraphPipelinesAMDX vkCreateExecutionGraphPipelinesAMDX = nullptr;
  PFN_vkGetExecutionGraphPipelineNodeIndexAMDX vkGetExecutionGraphPipelineNodeIndexAMDX = nullptr;
  PFN_vkGetExecutionGraphPipelineScratchSizeAMDX vkGetExecutionGraphPipelineScratchSizeAMDX = nullptr;
#else 
  ALIGNMENT_ARRAY(4, 7);
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_anti_lag)
  PFN_vkAntiLagUpdateAMD vkAntiLagUpdateAMD = nullptr;
#else 
  ALIGNMENT(5);
#endif /* defined(VK_AMD_anti_lag) */
#if defined(VK_AMD_buffer_marker)
  PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD = nullptr;
#else 
  ALIGNMENT(6);
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_display_native_hdr)
  PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD = nullptr;
#else 
  ALIGNMENT(7);
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
  PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD = nullptr;
  PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD = nullptr;
#else 
  ALIGNMENT_ARRAY(8, 2);
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
  PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD = nullptr;
#else 
  ALIGNMENT(9);
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
  PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID = nullptr;
  PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID = nullptr;
#else 
  ALIGNMENT_ARRAY(10, 2);
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_EXT_acquire_drm_display)
  PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT = nullptr;
  PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(11, 2);
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
  PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT = nullptr;
  PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(12, 2);
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
  PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT vkCmdSetAttachmentFeedbackLoopEnableEXT = nullptr;
#else 
  ALIGNMENT(13);
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
  PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT = nullptr;
#else 
  ALIGNMENT(14);
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
  PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT = nullptr;
  PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(15, 2);
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
  PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT = nullptr;
#else 
  ALIGNMENT(16);
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
  PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT = nullptr;
  PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(17, 2);
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_debug_marker)
  PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT = nullptr;
  PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT = nullptr;
  PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT = nullptr;
  PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT = nullptr;
  PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(18, 5);
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_debug_report)
  PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = nullptr;
  PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT = nullptr;
  PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(19, 3);
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
  PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT = nullptr;
  PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT = nullptr;
  PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT = nullptr;
  PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = nullptr;
  PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT = nullptr;
  PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT = nullptr;
  PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT = nullptr;
  PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT = nullptr;
  PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT = nullptr;
  PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT = nullptr;
  PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(20, 11);
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_depth_bias_control)
  PFN_vkCmdSetDepthBias2EXT vkCmdSetDepthBias2EXT = nullptr;
#else 
  ALIGNMENT(21);
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
  PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT vkCmdBindDescriptorBufferEmbeddedSamplersEXT = nullptr;
  PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT = nullptr;
  PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT = nullptr;
  PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT vkGetBufferOpaqueCaptureDescriptorDataEXT = nullptr;
  PFN_vkGetDescriptorEXT vkGetDescriptorEXT = nullptr;
  PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT = nullptr;
  PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT = nullptr;
  PFN_vkGetImageOpaqueCaptureDescriptorDataEXT vkGetImageOpaqueCaptureDescriptorDataEXT = nullptr;
  PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT vkGetImageViewOpaqueCaptureDescriptorDataEXT = nullptr;
  PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT vkGetSamplerOpaqueCaptureDescriptorDataEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(22, 10);
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
  PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT = nullptr;
#else 
  ALIGNMENT(23);
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
  PFN_vkGetDeviceFaultInfoEXT vkGetDeviceFaultInfoEXT = nullptr;
#else 
  ALIGNMENT(24);
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_device_generated_commands)
  PFN_vkCmdExecuteGeneratedCommandsEXT vkCmdExecuteGeneratedCommandsEXT = nullptr;
  PFN_vkCmdPreprocessGeneratedCommandsEXT vkCmdPreprocessGeneratedCommandsEXT = nullptr;
  PFN_vkCreateIndirectCommandsLayoutEXT vkCreateIndirectCommandsLayoutEXT = nullptr;
  PFN_vkCreateIndirectExecutionSetEXT vkCreateIndirectExecutionSetEXT = nullptr;
  PFN_vkDestroyIndirectCommandsLayoutEXT vkDestroyIndirectCommandsLayoutEXT = nullptr;
  PFN_vkDestroyIndirectExecutionSetEXT vkDestroyIndirectExecutionSetEXT = nullptr;
  PFN_vkGetGeneratedCommandsMemoryRequirementsEXT vkGetGeneratedCommandsMemoryRequirementsEXT = nullptr;
  PFN_vkUpdateIndirectExecutionSetPipelineEXT vkUpdateIndirectExecutionSetPipelineEXT = nullptr;
  PFN_vkUpdateIndirectExecutionSetShaderEXT vkUpdateIndirectExecutionSetShaderEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(25, 9);
#endif /* defined(VK_EXT_device_generated_commands) */
#if defined(VK_EXT_direct_mode_display)
  PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT = nullptr;
#else 
  ALIGNMENT(26);
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
  PFN_vkCreateDirectFBSurfaceEXT vkCreateDirectFBSurfaceEXT = nullptr;
  PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(27, 2);
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_discard_rectangles)
  PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT = nullptr;
#else 
  ALIGNMENT(28);
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
  PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT = nullptr;
  PFN_vkCmdSetDiscardRectangleModeEXT vkCmdSetDiscardRectangleModeEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(29, 2);
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
  PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT = nullptr;
  PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT = nullptr;
  PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT = nullptr;
  PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(30, 4);
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_display_surface_counter)
  PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT = nullptr;
#else 
  ALIGNMENT(31);
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_external_memory_host)
  PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT = nullptr;
#else 
  ALIGNMENT(32);
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_full_screen_exclusive)
  PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT = nullptr;
  PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT = nullptr;
  PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(33, 3);
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1))
  PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT = nullptr;
#else 
  ALIGNMENT(34);
#endif /* defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) */
#if defined(VK_EXT_hdr_metadata)
  PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT = nullptr;
#else 
  ALIGNMENT(35);
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_headless_surface)
  PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT = nullptr;
#else 
  ALIGNMENT(36);
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_host_image_copy)
  PFN_vkCopyImageToImageEXT vkCopyImageToImageEXT = nullptr;
  PFN_vkCopyImageToMemoryEXT vkCopyImageToMemoryEXT = nullptr;
  PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT = nullptr;
  PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(37, 4);
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
  PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT = nullptr;
#else 
  ALIGNMENT(38);
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
  PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT = nullptr;
#else 
  ALIGNMENT(39);
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
  PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT = nullptr;
#else 
  ALIGNMENT(40);
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_mesh_shader)
  PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT = nullptr;
  PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT = nullptr;
  PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(41, 3);
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_metal_objects)
  PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT = nullptr;
#else 
  ALIGNMENT(42);
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_metal_surface)
  PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT = nullptr;
#else 
  ALIGNMENT(43);
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_multi_draw)
  PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT = nullptr;
  PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(44, 2);
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
  PFN_vkBuildMicromapsEXT vkBuildMicromapsEXT = nullptr;
  PFN_vkCmdBuildMicromapsEXT vkCmdBuildMicromapsEXT = nullptr;
  PFN_vkCmdCopyMemoryToMicromapEXT vkCmdCopyMemoryToMicromapEXT = nullptr;
  PFN_vkCmdCopyMicromapEXT vkCmdCopyMicromapEXT = nullptr;
  PFN_vkCmdCopyMicromapToMemoryEXT vkCmdCopyMicromapToMemoryEXT = nullptr;
  PFN_vkCmdWriteMicromapsPropertiesEXT vkCmdWriteMicromapsPropertiesEXT = nullptr;
  PFN_vkCopyMemoryToMicromapEXT vkCopyMemoryToMicromapEXT = nullptr;
  PFN_vkCopyMicromapEXT vkCopyMicromapEXT = nullptr;
  PFN_vkCopyMicromapToMemoryEXT vkCopyMicromapToMemoryEXT = nullptr;
  PFN_vkCreateMicromapEXT vkCreateMicromapEXT = nullptr;
  PFN_vkDestroyMicromapEXT vkDestroyMicromapEXT = nullptr;
  PFN_vkGetDeviceMicromapCompatibilityEXT vkGetDeviceMicromapCompatibilityEXT = nullptr;
  PFN_vkGetMicromapBuildSizesEXT vkGetMicromapBuildSizesEXT = nullptr;
  PFN_vkWriteMicromapsPropertiesEXT vkWriteMicromapsPropertiesEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(45, 14);
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
  PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT = nullptr;
#else 
  ALIGNMENT(46);
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
  PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT = nullptr;
#else 
  ALIGNMENT(47);
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_private_data)
  PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT = nullptr;
  PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT = nullptr;
  PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT = nullptr;
  PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(48, 4);
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
  PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT = nullptr;
  PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(49, 2);
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
  PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT = nullptr;
  PFN_vkGetShaderModuleIdentifierEXT vkGetShaderModuleIdentifierEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(50, 2);
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
  PFN_vkCmdBindShadersEXT vkCmdBindShadersEXT = nullptr;
  PFN_vkCreateShadersEXT vkCreateShadersEXT = nullptr;
  PFN_vkDestroyShaderEXT vkDestroyShaderEXT = nullptr;
  PFN_vkGetShaderBinaryDataEXT vkGetShaderBinaryDataEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(51, 4);
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
  PFN_vkReleaseSwapchainImagesEXT vkReleaseSwapchainImagesEXT = nullptr;
#else 
  ALIGNMENT(52);
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_tooling_info)
  PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT = nullptr;
#else 
  ALIGNMENT(53);
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_EXT_transform_feedback)
  PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT = nullptr;
  PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT = nullptr;
  PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT = nullptr;
  PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT = nullptr;
  PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT = nullptr;
  PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(54, 6);
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
  PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT = nullptr;
  PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT = nullptr;
  PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT = nullptr;
  PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(55, 4);
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
  PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA = nullptr;
  PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA = nullptr;
  PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA = nullptr;
  PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA = nullptr;
  PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA = nullptr;
#else 
  ALIGNMENT_ARRAY(56, 5);
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
  PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA = nullptr;
  PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA = nullptr;
#else 
  ALIGNMENT_ARRAY(57, 2);
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
  PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA = nullptr;
  PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA = nullptr;
#else 
  ALIGNMENT_ARRAY(58, 2);
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_FUCHSIA_imagepipe_surface)
  PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA = nullptr;
#else 
  ALIGNMENT(59);
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
  PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP = nullptr;
#else 
  ALIGNMENT(60);
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_GOOGLE_display_timing)
  PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE = nullptr;
  PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE = nullptr;
#else 
  ALIGNMENT_ARRAY(61, 2);
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
  PFN_vkCmdDrawClusterHUAWEI vkCmdDrawClusterHUAWEI = nullptr;
  PFN_vkCmdDrawClusterIndirectHUAWEI vkCmdDrawClusterIndirectHUAWEI = nullptr;
#else 
  ALIGNMENT_ARRAY(62, 2);
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
  PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI = nullptr;
#else 
  ALIGNMENT(63);
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading)
  PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI = nullptr;
  PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI = nullptr;
#else 
  ALIGNMENT_ARRAY(64, 2);
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
  PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL = nullptr;
  PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL = nullptr;
  PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL = nullptr;
  PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL = nullptr;
  PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL = nullptr;
  PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL = nullptr;
  PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL = nullptr;
  PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL = nullptr;
  PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL = nullptr;
#else 
  ALIGNMENT_ARRAY(65, 9);
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
  PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR = nullptr;
  PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR = nullptr;
  PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR = nullptr;
  PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR = nullptr;
  PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR = nullptr;
  PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR = nullptr;
  PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR = nullptr;
  PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR = nullptr;
  PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR = nullptr;
  PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR = nullptr;
  PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR = nullptr;
  PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR = nullptr;
  PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR = nullptr;
  PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR = nullptr;
  PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR = nullptr;
  PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(66, 16);
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_android_surface)
  PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR = nullptr;
#else 
  ALIGNMENT(67);
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_bind_memory2)
  PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR = nullptr;
  PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(68, 2);
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
  PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR = nullptr;
  PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR = nullptr;
  PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(69, 3);
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
  PFN_vkGetCalibratedTimestampsKHR vkGetCalibratedTimestampsKHR = nullptr;
  PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR vkGetPhysicalDeviceCalibrateableTimeDomainsKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(70, 2);
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_cooperative_matrix)
  PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR = nullptr;
#else 
  ALIGNMENT(71);
#endif /* defined(VK_KHR_cooperative_matrix) */
#if defined(VK_KHR_copy_commands2)
  PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR = nullptr;
  PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR = nullptr;
  PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR = nullptr;
  PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR = nullptr;
  PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR = nullptr;
  PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(72, 6);
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_create_renderpass2)
  PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR = nullptr;
  PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR = nullptr;
  PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR = nullptr;
  PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(73, 4);
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
  PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR = nullptr;
  PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR = nullptr;
  PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR = nullptr;
  PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR = nullptr;
  PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(74, 5);
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
  PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR = nullptr;
  PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR = nullptr;
  PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(75, 3);
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
  PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR = nullptr;
  PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR = nullptr;
  PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(76, 3);
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_device_group_creation)
  PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR = nullptr;
#else 
  ALIGNMENT(77);
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
  PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR = nullptr;
  PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR = nullptr;
  PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR = nullptr;
  PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR = nullptr;
  PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR = nullptr;
  PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR = nullptr;
  PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(78, 7);
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_display_swapchain)
  PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR = nullptr;
#else 
  ALIGNMENT(79);
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
  PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR = nullptr;
  PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(80, 2);
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
  PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR = nullptr;
  PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(81, 2);
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
  PFN_vkCmdSetRenderingAttachmentLocationsKHR vkCmdSetRenderingAttachmentLocationsKHR = nullptr;
  PFN_vkCmdSetRenderingInputAttachmentIndicesKHR vkCmdSetRenderingInputAttachmentIndicesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(82, 2);
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_capabilities)
  PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR = nullptr;
#else 
  ALIGNMENT(83);
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_fence_fd)
  PFN_vkGetFenceFdKHR vkGetFenceFdKHR = nullptr;
  PFN_vkImportFenceFdKHR vkImportFenceFdKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(84, 2);
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
  PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR = nullptr;
  PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(85, 2);
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_capabilities)
  PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR = nullptr;
#else 
  ALIGNMENT(86);
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_memory_fd)
  PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR = nullptr;
  PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(87, 2);
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
  PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR = nullptr;
  PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(88, 2);
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_capabilities)
  PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR = nullptr;
#else 
  ALIGNMENT(89);
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_external_semaphore_fd)
  PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR = nullptr;
  PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(90, 2);
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
  PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR = nullptr;
  PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(91, 2);
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
  PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR = nullptr;
  PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(92, 2);
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
  PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR = nullptr;
  PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR = nullptr;
  PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(93, 4);
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_memory_requirements2)
  PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR = nullptr;
  PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR = nullptr;
  PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(94, 3);
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_get_physical_device_properties2)
  PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR = nullptr;
  PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR = nullptr;
  PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(95, 7);
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
  PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(96, 2);
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_line_rasterization)
  PFN_vkCmdSetLineStippleKHR vkCmdSetLineStippleKHR = nullptr;
#else 
  ALIGNMENT(97);
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
  PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR = nullptr;
#else 
  ALIGNMENT(98);
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance3)
  PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR = nullptr;
#else 
  ALIGNMENT(99);
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
  PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR = nullptr;
  PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR = nullptr;
  PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(100, 3);
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
  PFN_vkCmdBindIndexBuffer2KHR vkCmdBindIndexBuffer2KHR = nullptr;
  PFN_vkGetDeviceImageSubresourceLayoutKHR vkGetDeviceImageSubresourceLayoutKHR = nullptr;
  PFN_vkGetImageSubresourceLayout2KHR vkGetImageSubresourceLayout2KHR = nullptr;
  PFN_vkGetRenderingAreaGranularityKHR vkGetRenderingAreaGranularityKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(101, 4);
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
  PFN_vkCmdBindDescriptorSets2KHR vkCmdBindDescriptorSets2KHR = nullptr;
  PFN_vkCmdPushConstants2KHR vkCmdPushConstants2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(102, 2);
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
  PFN_vkCmdPushDescriptorSet2KHR vkCmdPushDescriptorSet2KHR = nullptr;
  PFN_vkCmdPushDescriptorSetWithTemplate2KHR vkCmdPushDescriptorSetWithTemplate2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(103, 2);
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
  PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT vkCmdBindDescriptorBufferEmbeddedSamplers2EXT = nullptr;
  PFN_vkCmdSetDescriptorBufferOffsets2EXT vkCmdSetDescriptorBufferOffsets2EXT = nullptr;
#else 
  ALIGNMENT_ARRAY(104, 2);
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
  PFN_vkMapMemory2KHR vkMapMemory2KHR = nullptr;
  PFN_vkUnmapMemory2KHR vkUnmapMemory2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(105, 2);
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
  PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR = nullptr;
  PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR = nullptr;
  PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR = nullptr;
  PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(106, 4);
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_binary)
  PFN_vkCreatePipelineBinariesKHR vkCreatePipelineBinariesKHR = nullptr;
  PFN_vkDestroyPipelineBinaryKHR vkDestroyPipelineBinaryKHR = nullptr;
  PFN_vkGetPipelineBinaryDataKHR vkGetPipelineBinaryDataKHR = nullptr;
  PFN_vkGetPipelineKeyKHR vkGetPipelineKeyKHR = nullptr;
  PFN_vkReleaseCapturedPipelineDataKHR vkReleaseCapturedPipelineDataKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(107, 5);
#endif /* defined(VK_KHR_pipeline_binary) */
#if defined(VK_KHR_pipeline_executable_properties)
  PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR = nullptr;
  PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR = nullptr;
  PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(108, 3);
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
  PFN_vkWaitForPresentKHR vkWaitForPresentKHR = nullptr;
#else 
  ALIGNMENT(109);
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_push_descriptor)
  PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR = nullptr;
#else 
  ALIGNMENT(110);
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
  PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR = nullptr;
#else 
  ALIGNMENT(111);
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
  PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR = nullptr;
  PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR = nullptr;
  PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR = nullptr;
  PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR = nullptr;
  PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR = nullptr;
  PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR = nullptr;
  PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(112, 7);
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
  PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR = nullptr;
  PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(113, 2);
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
  PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR = nullptr;
#else 
  ALIGNMENT(114);
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_surface)
  PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR = nullptr;
  PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(115, 5);
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_swapchain)
  PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR = nullptr;
  PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR = nullptr;
  PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR = nullptr;
  PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR = nullptr;
  PFN_vkQueuePresentKHR vkQueuePresentKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(116, 5);
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_synchronization2)
  PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR = nullptr;
  PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR = nullptr;
  PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR = nullptr;
  PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR = nullptr;
  PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR = nullptr;
  PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR = nullptr;
#else 
  ALIGNMENT_ARRAY(117, 6);
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker)
  PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD = nullptr;
#else 
  ALIGNMENT(118);
#endif /* defined(VK_KHR_synchronization2) && defined(VK_AMD_buffer_marker) */
#if defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints)
  PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV = nullptr;
#else 
  ALIGNMENT(119);
#endif /* defined(VK_KHR_synchronization2) && defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_KHR_timeline_semaphore)
  PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR = nullptr;
  PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR = nullptr;
  PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(120, 3);
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
  PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR = nullptr;
#else 
  ALIGNMENT(121);
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
  PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR = nullptr;
  PFN_vkGetEncodedVideoSessionParametersKHR vkGetEncodedVideoSessionParametersKHR = nullptr;
  PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(122, 3);
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
  PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR = nullptr;
  PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR = nullptr;
  PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR = nullptr;
  PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR = nullptr;
  PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR = nullptr;
  PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR = nullptr;
  PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR = nullptr;
  PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR = nullptr;
  PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR = nullptr;
  PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR = nullptr;
  PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR = nullptr;
  PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(123, 12);
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
  PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(124, 2);
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
  PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(125, 2);
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
  PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(126, 2);
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
  PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR = nullptr;
  PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(127, 2);
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
  PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK = nullptr;
#else 
  ALIGNMENT(128);
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
  PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK = nullptr;
#else 
  ALIGNMENT(129);
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
  PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN = nullptr;
#else 
  ALIGNMENT(130);
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NVX_binary_import)
  PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX = nullptr;
  PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX = nullptr;
  PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX = nullptr;
  PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX = nullptr;
  PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX = nullptr;
#else 
  ALIGNMENT_ARRAY(131, 5);
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
  PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX = nullptr;
  PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX = nullptr;
#else 
  ALIGNMENT_ARRAY(132, 2);
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NV_acquire_winrt_display)
  PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV = nullptr;
  PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV = nullptr;
#else 
  ALIGNMENT_ARRAY(133, 2);
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_clip_space_w_scaling)
  PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV = nullptr;
#else 
  ALIGNMENT(134);
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_cooperative_matrix)
  PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV = nullptr;
#else 
  ALIGNMENT(135);
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_copy_memory_indirect)
  PFN_vkCmdCopyMemoryIndirectNV vkCmdCopyMemoryIndirectNV = nullptr;
  PFN_vkCmdCopyMemoryToImageIndirectNV vkCmdCopyMemoryToImageIndirectNV = nullptr;
#else 
  ALIGNMENT_ARRAY(136, 2);
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_coverage_reduction_mode)
  PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV = nullptr;
#else 
  ALIGNMENT(137);
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_cuda_kernel_launch)
  PFN_vkCmdCudaLaunchKernelNV vkCmdCudaLaunchKernelNV = nullptr;
  PFN_vkCreateCudaFunctionNV vkCreateCudaFunctionNV = nullptr;
  PFN_vkCreateCudaModuleNV vkCreateCudaModuleNV = nullptr;
  PFN_vkDestroyCudaFunctionNV vkDestroyCudaFunctionNV = nullptr;
  PFN_vkDestroyCudaModuleNV vkDestroyCudaModuleNV = nullptr;
  PFN_vkGetCudaModuleCacheNV vkGetCudaModuleCacheNV = nullptr;
#else 
  ALIGNMENT_ARRAY(138, 6);
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
  PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV = nullptr;
  PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV = nullptr;
#else 
  ALIGNMENT_ARRAY(139, 2);
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_generated_commands)
  PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV = nullptr;
  PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV = nullptr;
  PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV = nullptr;
  PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV = nullptr;
  PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV = nullptr;
  PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV = nullptr;
#else 
  ALIGNMENT_ARRAY(140, 6);
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
  PFN_vkCmdUpdatePipelineIndirectBufferNV vkCmdUpdatePipelineIndirectBufferNV = nullptr;
  PFN_vkGetPipelineIndirectDeviceAddressNV vkGetPipelineIndirectDeviceAddressNV = nullptr;
  PFN_vkGetPipelineIndirectMemoryRequirementsNV vkGetPipelineIndirectMemoryRequirementsNV = nullptr;
#else 
  ALIGNMENT_ARRAY(141, 3);
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_memory_capabilities)
  PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV = nullptr;
#else 
  ALIGNMENT(142);
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_external_memory_rdma)
  PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV = nullptr;
#else 
  ALIGNMENT(143);
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
  PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV = nullptr;
#else 
  ALIGNMENT(144);
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
  PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV = nullptr;
#else 
  ALIGNMENT(145);
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
  PFN_vkGetLatencyTimingsNV vkGetLatencyTimingsNV = nullptr;
  PFN_vkLatencySleepNV vkLatencySleepNV = nullptr;
  PFN_vkQueueNotifyOutOfBandNV vkQueueNotifyOutOfBandNV = nullptr;
  PFN_vkSetLatencyMarkerNV vkSetLatencyMarkerNV = nullptr;
  PFN_vkSetLatencySleepModeNV vkSetLatencySleepModeNV = nullptr;
#else 
  ALIGNMENT_ARRAY(146, 5);
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
  PFN_vkCmdDecompressMemoryIndirectCountNV vkCmdDecompressMemoryIndirectCountNV = nullptr;
  PFN_vkCmdDecompressMemoryNV vkCmdDecompressMemoryNV = nullptr;
#else 
  ALIGNMENT_ARRAY(147, 2);
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
  PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV = nullptr;
  PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV = nullptr;
  PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV = nullptr;
#else 
  ALIGNMENT_ARRAY(148, 3);
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_optical_flow)
  PFN_vkBindOpticalFlowSessionImageNV vkBindOpticalFlowSessionImageNV = nullptr;
  PFN_vkCmdOpticalFlowExecuteNV vkCmdOpticalFlowExecuteNV = nullptr;
  PFN_vkCreateOpticalFlowSessionNV vkCreateOpticalFlowSessionNV = nullptr;
  PFN_vkDestroyOpticalFlowSessionNV vkDestroyOpticalFlowSessionNV = nullptr;
  PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV vkGetPhysicalDeviceOpticalFlowImageFormatsNV = nullptr;
#else 
  ALIGNMENT_ARRAY(149, 5);
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_ray_tracing)
  PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV = nullptr;
  PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV = nullptr;
  PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV = nullptr;
  PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV = nullptr;
  PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV = nullptr;
  PFN_vkCompileDeferredNV vkCompileDeferredNV = nullptr;
  PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV = nullptr;
  PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV = nullptr;
  PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV = nullptr;
  PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV = nullptr;
  PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV = nullptr;
  PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV = nullptr;
#else 
  ALIGNMENT_ARRAY(150, 12);
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
  PFN_vkCmdSetExclusiveScissorEnableNV vkCmdSetExclusiveScissorEnableNV = nullptr;
#else 
  ALIGNMENT(151);
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
  PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV = nullptr;
#else 
  ALIGNMENT(152);
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
  PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV = nullptr;
  PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV = nullptr;
  PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV = nullptr;
#else 
  ALIGNMENT_ARRAY(153, 3);
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_QCOM_tile_properties)
  PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM = nullptr;
  PFN_vkGetFramebufferTilePropertiesQCOM vkGetFramebufferTilePropertiesQCOM = nullptr;
#else 
  ALIGNMENT_ARRAY(154, 2);
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QNX_external_memory_screen_buffer)
  PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX = nullptr;
#else 
  ALIGNMENT(155);
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_QNX_screen_surface)
  PFN_vkCreateScreenSurfaceQNX vkCreateScreenSurfaceQNX = nullptr;
  PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX = nullptr;
#else 
  ALIGNMENT_ARRAY(156, 2);
#endif /* defined(VK_QNX_screen_surface) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
  PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE = nullptr;
  PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE = nullptr;
#else 
  ALIGNMENT_ARRAY(157, 2);
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control))
  PFN_vkCmdSetDepthClampRangeEXT vkCmdSetDepthClampRangeEXT = nullptr;
#else 
  ALIGNMENT(158);
#endif /* (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
  PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT = nullptr;
  PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT = nullptr;
  PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT = nullptr;
  PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT = nullptr;
  PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT = nullptr;
  PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT = nullptr;
  PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT = nullptr;
  PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT = nullptr;
  PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT = nullptr;
  PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT = nullptr;
  PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT = nullptr;
  PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(159, 12);
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
  PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT = nullptr;
  PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT = nullptr;
  PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT = nullptr;
  PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT = nullptr;
  PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(160, 5);
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
  PFN_vkCmdSetAlphaToCoverageEnableEXT vkCmdSetAlphaToCoverageEnableEXT = nullptr;
  PFN_vkCmdSetAlphaToOneEnableEXT vkCmdSetAlphaToOneEnableEXT = nullptr;
  PFN_vkCmdSetColorBlendEnableEXT vkCmdSetColorBlendEnableEXT = nullptr;
  PFN_vkCmdSetColorBlendEquationEXT vkCmdSetColorBlendEquationEXT = nullptr;
  PFN_vkCmdSetColorWriteMaskEXT vkCmdSetColorWriteMaskEXT = nullptr;
  PFN_vkCmdSetDepthClampEnableEXT vkCmdSetDepthClampEnableEXT = nullptr;
  PFN_vkCmdSetLogicOpEnableEXT vkCmdSetLogicOpEnableEXT = nullptr;
  PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT = nullptr;
  PFN_vkCmdSetRasterizationSamplesEXT vkCmdSetRasterizationSamplesEXT = nullptr;
  PFN_vkCmdSetSampleMaskEXT vkCmdSetSampleMaskEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(161, 10);
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
  PFN_vkCmdSetTessellationDomainOriginEXT vkCmdSetTessellationDomainOriginEXT = nullptr;
#else 
  ALIGNMENT(162);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
  PFN_vkCmdSetRasterizationStreamEXT vkCmdSetRasterizationStreamEXT = nullptr;
#else 
  ALIGNMENT(163);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
  PFN_vkCmdSetConservativeRasterizationModeEXT vkCmdSetConservativeRasterizationModeEXT = nullptr;
  PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT vkCmdSetExtraPrimitiveOverestimationSizeEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(164, 2);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
  PFN_vkCmdSetDepthClipEnableEXT vkCmdSetDepthClipEnableEXT = nullptr;
#else 
  ALIGNMENT(165);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
  PFN_vkCmdSetSampleLocationsEnableEXT vkCmdSetSampleLocationsEnableEXT = nullptr;
#else 
  ALIGNMENT(166);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
  PFN_vkCmdSetColorBlendAdvancedEXT vkCmdSetColorBlendAdvancedEXT = nullptr;
#else 
  ALIGNMENT(167);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
  PFN_vkCmdSetProvokingVertexModeEXT vkCmdSetProvokingVertexModeEXT = nullptr;
#else 
  ALIGNMENT(168);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
  PFN_vkCmdSetLineRasterizationModeEXT vkCmdSetLineRasterizationModeEXT = nullptr;
  PFN_vkCmdSetLineStippleEnableEXT vkCmdSetLineStippleEnableEXT = nullptr;
#else 
  ALIGNMENT_ARRAY(169, 2);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
  PFN_vkCmdSetDepthClipNegativeOneToOneEXT vkCmdSetDepthClipNegativeOneToOneEXT = nullptr;
#else 
  ALIGNMENT(170);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
  PFN_vkCmdSetViewportWScalingEnableNV vkCmdSetViewportWScalingEnableNV = nullptr;
#else 
  ALIGNMENT(171);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
  PFN_vkCmdSetViewportSwizzleNV vkCmdSetViewportSwizzleNV = nullptr;
#else 
  ALIGNMENT(172);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
  PFN_vkCmdSetCoverageToColorEnableNV vkCmdSetCoverageToColorEnableNV = nullptr;
  PFN_vkCmdSetCoverageToColorLocationNV vkCmdSetCoverageToColorLocationNV = nullptr;
#else 
  ALIGNMENT_ARRAY(173, 2);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
  PFN_vkCmdSetCoverageModulationModeNV vkCmdSetCoverageModulationModeNV = nullptr;
  PFN_vkCmdSetCoverageModulationTableEnableNV vkCmdSetCoverageModulationTableEnableNV = nullptr;
  PFN_vkCmdSetCoverageModulationTableNV vkCmdSetCoverageModulationTableNV = nullptr;
#else 
  ALIGNMENT_ARRAY(174, 3);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
  PFN_vkCmdSetShadingRateImageEnableNV vkCmdSetShadingRateImageEnableNV = nullptr;
#else 
  ALIGNMENT(175);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
  PFN_vkCmdSetRepresentativeFragmentTestEnableNV vkCmdSetRepresentativeFragmentTestEnableNV = nullptr;
#else 
  ALIGNMENT(176);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
  PFN_vkCmdSetCoverageReductionModeNV vkCmdSetCoverageReductionModeNV = nullptr;
#else 
  ALIGNMENT(177);
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
  PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT = nullptr;
#else 
  ALIGNMENT(178);
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
  PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT = nullptr;
#else 
  ALIGNMENT(179);
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template)))
  PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR = nullptr;
#else 
  ALIGNMENT(180);
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template))) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
  PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR = nullptr;
  PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR = nullptr;
  PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR = nullptr;
#else 
  ALIGNMENT_ARRAY(181, 3);
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
  PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR = nullptr;
#else 
  ALIGNMENT(182);
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
  /* VOLK_CPP_GENERATE_CLASS_FUNCTION_POINTERS */
  
#undef ALIGNMENT
#undef ALIGNMENT_ARRAY

private:
  void genLoadLoader(void* context, PFN_vkVoidFunction (Volk::*load)(void*, char const*)) noexcept;
  void genLoadInstance(void* context, PFN_vkVoidFunction (Volk::*load)(void*, char const*)) noexcept;
  void genLoadDevice(void* context, PFN_vkVoidFunction (Volk::*load)(void*, char const*)) noexcept;
  [[nodiscard]] PFN_vkVoidFunction vkGetInstanceProcAddrStub(void* context, char const* name) noexcept;
  [[nodiscard]] PFN_vkVoidFunction vkGetDeviceProcAddrStub(void* context, char const* name) noexcept;
  [[nodiscard]] PFN_vkVoidFunction nullProcAddrStub(void* context, char const* name) noexcept;
  
  VkResult status_ = VK_SUCCESS;
  void * loadedModule_ = nullptr;
  VkInstance loadedInstance_ = VK_NULL_HANDLE;
  VkDevice loadedDevice_ = VK_NULL_HANDLE;
};

#ifdef VOLK_CPP_IMPLEMENTATION
#undef VOLK_CPP_IMPLEMENTATION
/* Prevent tools like dependency checkers from detecting a cyclic dependency */
#define VOLK_CPP_SOURCE "volk.cpp"
#include VOLK_CPP_SOURCE
#endif

/**
 * Copyright (c) 2018-2023 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
/* clang-format on */
