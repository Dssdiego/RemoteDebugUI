#pragma once

#include <imgui/imgui.h>
#include <imgui/imgui_impl_vulkan.h>

#include <vulkan/vulkan.h>

class Renderer
{
public:
    
    bool Run(int width, int height, float scale);
    
private:
    
    static void CheckVKResult(VkResult err);
    
    bool IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension);
    
    void SetupVulkan(ImVector<const char*> instance_extensions);
    void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
    
    void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
    void FramePresent(ImGui_ImplVulkanH_Window* wd);
    
    void CleanupVulkanWindow();
    void CleanupVulkan();
    
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    int width = 0, height = 0;
    float renderScale = 0;
    
    VkAllocationCallbacks*   g_Allocator = nullptr;
    VkInstance               g_Instance = VK_NULL_HANDLE;
    VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice                 g_Device = VK_NULL_HANDLE;
    uint32_t                 g_QueueFamily = (uint32_t)-1;
    VkQueue                  g_Queue = VK_NULL_HANDLE;
    VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
    VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;

    ImGui_ImplVulkanH_Window g_MainWindowData;
    uint32_t                 g_MinImageCount = 2;
    bool                     g_SwapChainRebuild = false;
};
