#include "Renderer.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include <imgui/imgui_impl_sdl3.h>

#include "Window.h"

#include "../UI/DebugUI.h"
#include "../Logging/Logger.h"

void Renderer::Init()
{
    ImVector<const char*> extensions;
    {
        uint32_t sdl_extensions_count = 0;
        const char* const* sdl_extensions = SDL_Vulkan_GetInstanceExtensions(&sdl_extensions_count);
        for (uint32_t n = 0; n < sdl_extensions_count; n++)
        {
            extensions.push_back(sdl_extensions[n]);
        }
    }
    
    SetupVulkan(extensions);
    
    Logger::Info("Renderer initialized");
}

void Renderer::SetupBackend()
{
     // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(renderScale);        
    style.FontScaleDpi = renderScale;       

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForVulkan(Window::GetSDLWindow());
    ImGui_ImplVulkan_InitInfo init_info = {};
    //init_info.ApiVersion = VK_API_VERSION_1_3;              // Pass in your value of VkApplicationInfo::apiVersion, otherwise will default to header version.
    init_info.Instance = mInstance;
    init_info.PhysicalDevice = mPhysicalDevice;
    init_info.Device = mDevice;
    init_info.QueueFamily = mQueueFamily;
    init_info.Queue = mQueue;
    init_info.PipelineCache = mPipelineCache;
    init_info.DescriptorPool = mDescriptorPool;
    init_info.MinImageCount = GetMinImageCount();
    init_info.ImageCount = mVulkanWindow->ImageCount;
    init_info.Allocator = mAllocator;
    init_info.PipelineInfoMain.RenderPass = mVulkanWindow->RenderPass;
    init_info.PipelineInfoMain.Subpass = 0;
    init_info.PipelineInfoMain.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.CheckVkResultFn = CheckVKResult;
    ImGui_ImplVulkan_Init(&init_info);
    
    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will call AddFontDefault() to select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefaultVector();
    //io.Fonts->AddFontDefaultBitmap();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);
}

void Renderer::CreateVulkanWindow(VkSurfaceKHR surface, int width, int height)
{
    mVulkanWindow = &mVulkanWindowData;
    mVulkanWindow->Surface = surface;

    // Check for WSI support
    VkBool32 res;
    vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice, mQueueFamily, mVulkanWindow->Surface, &res);
    if (res != VK_TRUE)
    {
        Logger::Error("Error no WSI support on physical device 0\n");
        exit(-1);
    }

    // Select Surface Format
    const VkFormat requestSurfaceImageFormat[] = { VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM };
    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    mVulkanWindow->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(mPhysicalDevice, mVulkanWindow->Surface, requestSurfaceImageFormat, (size_t)IM_COUNTOF(requestSurfaceImageFormat), requestSurfaceColorSpace);

    // Select Present Mode
#ifdef APP_USE_UNLIMITED_FRAME_RATE
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };
#else
    VkPresentModeKHR present_modes[] = { VK_PRESENT_MODE_FIFO_KHR };
#endif
    mVulkanWindow->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(mPhysicalDevice, mVulkanWindow->Surface, &present_modes[0], IM_COUNTOF(present_modes));
    //printf("[vulkan] Selected PresentMode = %d\n", mVulkanWindow->PresentMode);

    // Create SwapChain, RenderPass, Framebuffer, etc.
    IM_ASSERT(GetMinImageCount() >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(mInstance, mPhysicalDevice, mDevice, mVulkanWindow, mQueueFamily, mAllocator, width, height, GetMinImageCount(), 0);
}

void Renderer::CheckResizeSwapChain(int width, int height)
{
    if (width > 0 && height > 0 && (mSwapChainRebuild || mVulkanWindowData.Width != width || mVulkanWindowData.Height != height))
    {
        ImGui_ImplVulkan_SetMinImageCount(mMinImageCount);
        ImGui_ImplVulkanH_CreateOrResizeWindow(mInstance, mPhysicalDevice, mDevice, mVulkanWindow, mQueueFamily, mAllocator, width, height, mMinImageCount, 0);
        mVulkanWindowData.FrameIndex = 0;
        mSwapChainRebuild = false;
    }   
}

void Renderer::Render()
{
    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    DebugUI::Render();

    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();
    const bool is_minimized = (draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f);
    if (!is_minimized)
    {
        mVulkanWindow->ClearValue.color.float32[0] = clearColor.x * clearColor.w;
        mVulkanWindow->ClearValue.color.float32[1] = clearColor.y * clearColor.w;
        mVulkanWindow->ClearValue.color.float32[2] = clearColor.z * clearColor.w;
        mVulkanWindow->ClearValue.color.float32[3] = clearColor.w;
        FrameRender(mVulkanWindow, draw_data);
        FramePresent(mVulkanWindow);
    }
}

void Renderer::Shutdown()
{
    // Cleanup
    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
    const VkResult err = vkDeviceWaitIdle(mDevice);
    CheckVKResult(err);
    
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    CleanupVulkanWindow();
    CleanupVulkan();
    
    Logger::Info("Renderer shutdown");
}

void Renderer::CheckVKResult(const VkResult err)
{
    if (err == VK_SUCCESS)
    {
        return;
    }
    
    (stderr, "[vulkan] Error: VkResult = %d\n", err);
    
    if (err < 0)
    {
        abort();
    }
}

bool Renderer::IsExtensionAvailable(const ImVector<VkExtensionProperties>& properties, const char* extension)
{
    for (const VkExtensionProperties& p : properties)
    {
        if (strcmp(p.extensionName, extension) == 0)
        {
            return true;
        }
    }
    
    return false;
}

void Renderer::SetupVulkan(ImVector<const char*> instance_extensions)
{
    VkResult err;
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
    volkInitialize();
#endif

    // Create Vulkan Instance
    {
        VkInstanceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        // Enumerate available extensions
        uint32_t properties_count;
        ImVector<VkExtensionProperties> properties;
        vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, nullptr);
        properties.resize(properties_count);
        err = vkEnumerateInstanceExtensionProperties(nullptr, &properties_count, properties.Data);
        CheckVKResult(err);

        // Enable required extensions
        if (IsExtensionAvailable(properties, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME))
            instance_extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#ifdef VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
        if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME))
        {
            instance_extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            create_info.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
        }
#endif

        // Enabling validation layers
#ifdef APP_USE_VULKAN_DEBUG_REPORT
        const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
        create_info.enabledLayerCount = 1;
        create_info.ppEnabledLayerNames = layers;
        instance_extensions.push_back("VK_EXT_debug_report");
#endif

        // Create Vulkan Instance
        create_info.enabledExtensionCount = (uint32_t)instance_extensions.Size;
        create_info.ppEnabledExtensionNames = instance_extensions.Data;
        err = vkCreateInstance(&create_info, mAllocator, &mInstance);
        CheckVKResult(err);
#ifdef IMGUI_IMPL_VULKAN_USE_VOLK
        volkLoadInstance(mInstance);
#endif
    }

    // Select Physical Device (GPU)
    mPhysicalDevice = ImGui_ImplVulkanH_SelectPhysicalDevice(mInstance);
    IM_ASSERT(mPhysicalDevice != VK_NULL_HANDLE);

    // Select graphics queue family
    mQueueFamily = ImGui_ImplVulkanH_SelectQueueFamilyIndex(mPhysicalDevice);
    IM_ASSERT(mQueueFamily != (uint32_t)-1);

    // Create Logical Device (with 1 queue)
    {
        ImVector<const char*> device_extensions;
        device_extensions.push_back("VK_KHR_swapchain");

        // Enumerate physical device extension
        uint32_t properties_count;
        ImVector<VkExtensionProperties> properties;
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &properties_count, nullptr);
        properties.resize(properties_count);
        vkEnumerateDeviceExtensionProperties(mPhysicalDevice, nullptr, &properties_count, properties.Data);
#ifdef VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
        if (IsExtensionAvailable(properties, VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME))
            device_extensions.push_back(VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME);
#endif

        const float queue_priority[] = { 1.0f };
        VkDeviceQueueCreateInfo queue_info[1] = {};
        queue_info[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info[0].queueFamilyIndex = mQueueFamily;
        queue_info[0].queueCount = 1;
        queue_info[0].pQueuePriorities = queue_priority;
        VkDeviceCreateInfo create_info = {};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = sizeof(queue_info) / sizeof(queue_info[0]);
        create_info.pQueueCreateInfos = queue_info;
        create_info.enabledExtensionCount = (uint32_t)device_extensions.Size;
        create_info.ppEnabledExtensionNames = device_extensions.Data;
        err = vkCreateDevice(mPhysicalDevice, &create_info, mAllocator, &mDevice);
        CheckVKResult(err);
        vkGetDeviceQueue(mDevice, mQueueFamily, 0, &mQueue);
    }

    // Create Descriptor Pool
    // If you wish to load e.g. additional textures you may need to alter pools sizes and maxSets.
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, IMGUI_IMPL_VULKAN_MINIMUM_IMAGE_SAMPLER_POOL_SIZE },
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 0;
        for (VkDescriptorPoolSize& pool_size : pool_sizes)
            pool_info.maxSets += pool_size.descriptorCount;
        pool_info.poolSizeCount = (uint32_t)IM_COUNTOF(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        err = vkCreateDescriptorPool(mDevice, &pool_info, mAllocator, &mDescriptorPool);
        CheckVKResult(err);
    }
}

void Renderer::SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height)
{

}

void Renderer::FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data)
{
    VkSemaphore image_acquired_semaphore  = mVulkanWindow->FrameSemaphores[mVulkanWindow->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = mVulkanWindow->FrameSemaphores[mVulkanWindow->SemaphoreIndex].RenderCompleteSemaphore;
    VkResult err = vkAcquireNextImageKHR(mDevice, mVulkanWindow->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &mVulkanWindow->FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        mSwapChainRebuild = true;
    }
    
    if (err == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return;
    }
    
    if (err != VK_SUBOPTIMAL_KHR)
    {
        CheckVKResult(err);
    }

    // waiting for fences
    ImGui_ImplVulkanH_Frame* fd = &mVulkanWindow->Frames[mVulkanWindow->FrameIndex];
    {
        err = vkWaitForFences(mDevice, 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
        CheckVKResult(err);

        err = vkResetFences(mDevice, 1, &fd->Fence);
        CheckVKResult(err);
    }
    
    // command buffer begin
    {
        err = vkResetCommandPool(mDevice, fd->CommandPool, 0);
        CheckVKResult(err);
        
        VkCommandBufferBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        CheckVKResult(err);
    }
    
    // render pass begin
    {
        VkRenderPassBeginInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass = mVulkanWindow->RenderPass;
        info.framebuffer = fd->Framebuffer;
        info.renderArea.extent.width = mVulkanWindow->Width;
        info.renderArea.extent.height = mVulkanWindow->Height;
        info.clearValueCount = 1;
        info.pClearValues = &mVulkanWindow->ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

    // submit command buffer / end render pass
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount = 1;
        info.pWaitSemaphores = &image_acquired_semaphore;
        info.pWaitDstStageMask = &wait_stage;
        info.commandBufferCount = 1;
        info.pCommandBuffers = &fd->CommandBuffer;
        info.signalSemaphoreCount = 1;
        info.pSignalSemaphores = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        CheckVKResult(err);
        
        err = vkQueueSubmit(mQueue, 1, &info, fd->Fence);
        CheckVKResult(err);
    }
}

void Renderer::FramePresent(ImGui_ImplVulkanH_Window* wd)
{
    if (mSwapChainRebuild)
    {
        return;
    }
    
    VkSemaphore render_complete_semaphore = mVulkanWindow->FrameSemaphores[mVulkanWindow->SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info = {};
    info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount = 1;
    info.pWaitSemaphores = &render_complete_semaphore;
    info.swapchainCount = 1;
    info.pSwapchains = &mVulkanWindow->Swapchain;
    info.pImageIndices = &mVulkanWindow->FrameIndex;
    
    VkResult err = vkQueuePresentKHR(mQueue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
    {
        mSwapChainRebuild = true;
    }
    
    if (err == VK_ERROR_OUT_OF_DATE_KHR)
    {
        return;
    }
    
    if (err != VK_SUBOPTIMAL_KHR)
    {
        CheckVKResult(err);
    }
    
    mVulkanWindow->SemaphoreIndex = (mVulkanWindow->SemaphoreIndex + 1) % mVulkanWindow->SemaphoreCount; // Now we can use the next set of semaphores
}

void Renderer::CleanupVulkanWindow()
{
    ImGui_ImplVulkanH_DestroyWindow(mInstance, mDevice, &mVulkanWindowData, mAllocator);
}

void Renderer::CleanupVulkan()
{
    vkDestroyDescriptorPool(mDevice, mDescriptorPool, mAllocator);

#ifdef APP_USE_VULKAN_DEBUG_REPORT
    // Remove the debug report callback
    auto f_vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(mInstance, "vkDestroyDebugReportCallbackEXT");
    f_vkDestroyDebugReportCallbackEXT(mInstance, g_DebugReport, mAllocator);
#endif // APP_USE_VULKAN_DEBUG_REPORT

    vkDestroyDevice(mDevice, mAllocator);
    vkDestroyInstance(mInstance, mAllocator);
}
