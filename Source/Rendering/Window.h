#pragma once

#include <SDL3/SDL.h>

class Window
{
public:
    
    static void Init();
    
    static bool ShouldClose() { return mShouldClose; }

    static SDL_Window* GetSDLWindow() { return mWindow; }
    
    static void Update();
    static void Shutdown();
    
private:
    
    inline static bool mShouldClose = false;
    
    inline static int width = 1080;
    inline static int height = 720;
    
    inline static SDL_Window* mWindow = nullptr;
    
};
