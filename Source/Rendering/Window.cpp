#include "Window.h"

#include <imgui/imgui_impl_sdl3.h>

#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.h>

#include "Renderer.h"
#include "../Logging/Logger.h"

void Window::Init()
{
    // Setup SDL
    // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts would likely be your SDL_AppInit() function]
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        Logger::Error("Error: SDL_Init(): %s\n", SDL_GetError());
    }

    // Create window with Vulkan graphics context
    SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    mWindow = SDL_CreateWindow("Remote Debug UI", width, height, window_flags);
    if (mWindow == nullptr)
    {
        Logger::Error("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
    }
    
    Renderer::Init();
    
    // Create Window Surface
    VkSurfaceKHR surface;
    VkResult err;
    if (SDL_Vulkan_CreateSurface(mWindow, Renderer::GetInstance(), Renderer::GetAllocator(), &surface) == 0)
    {
        Logger::Error("Failed to create Vulkan surface.\n");
    }

    // Create Framebuffers
    int w, h;
    SDL_GetWindowSize(mWindow, &w, &h);
    Renderer::CreateVulkanWindow(surface, w, h);
    
    Renderer::SetupBackend();
    
    SDL_SetWindowPosition(mWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(mWindow);
    
    Logger::Info("Window initialized");
}

void Window::Update()
{
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    // [If using SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your SDL_AppEvent() function]
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT)
        {
            mShouldClose = true;
        }
        
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(mWindow))
        {
            mShouldClose = true;
        }
    }

    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
    if (SDL_GetWindowFlags(mWindow) & SDL_WINDOW_MINIMIZED)
    {
        SDL_Delay(10);
        // continue;
    }

    // Resize swap chain?
    int fb_width, fb_height;
    SDL_GetWindowSize(mWindow, &fb_width, &fb_height);
    Renderer::CheckResizeSwapChain(fb_width,  fb_height); 
}

void Window::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
    
    Logger::Info("Window shutdown");
}
