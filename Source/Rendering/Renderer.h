#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>

#include <vulkan/vulkan.h>


class Renderer
{
public:
    
    static void Init();
    static void SetupBackend();
    
    static void CreateVulkanWindow(VkSurfaceKHR surface, int width, int height);
    static void CheckResizeSwapChain(int width, int height);
    
    static void ForceRebuildSwapChain();
    
    static void Render();
    
    static void Shutdown();
    
public:
    
    static VkAllocationCallbacks* GetAllocator() { return mAllocator; }
    static VkInstance GetInstance() { return mInstance; }
    static VkPhysicalDevice* GetPhysicalDevice() { return &mPhysicalDevice; }
    static VkDevice* GetDevice() { return &mDevice; }
    static VkQueue* GetQueue() { return &mQueue; }
    static uint32_t* GetQueueFamily() { return &mQueueFamily; }
    static VkPipelineCache* GetPipelineCache() { return &mPipelineCache; }
    static VkDescriptorPool* GetDescriptorPool() { return &mDescriptorPool; }
    
    static uint32_t GetMinImageCount() { return mMinImageCount; }
    
private:
    
    inline static VkAllocationCallbacks*     mAllocator = nullptr;
    inline static VkInstance                 mInstance = VK_NULL_HANDLE;
    inline static VkPhysicalDevice           mPhysicalDevice = VK_NULL_HANDLE;
    inline static VkDevice                   mDevice = VK_NULL_HANDLE;
    inline static uint32_t                   mQueueFamily = (uint32_t)-1;
    inline static VkQueue                    mQueue = VK_NULL_HANDLE;
    inline static VkPipelineCache            mPipelineCache = VK_NULL_HANDLE;
    inline static VkDescriptorPool           mDescriptorPool = VK_NULL_HANDLE;

    inline static ImGui_ImplVulkanH_Window*  mVulkanWindow;
    inline static ImGui_ImplVulkanH_Window   mVulkanWindowData;
    inline static uint32_t                   mMinImageCount = 2;
    inline static bool                       mSwapChainRebuild = false;
    
private:
    
    static void CheckVKResult(VkResult err);
    
    static bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
    
    static void SetupVulkan(ImVector<const char*> instance_extensions);
    static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
    
    static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    static void FramePresent(ImGui_ImplVulkanH_Window* wd);
    
    static void CleanupVulkanWindow();
    static void CleanupVulkan();
    
};
