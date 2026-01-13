#pragma once

#include <imgui/imgui.h>

class DebugUI
{
public:
    
    static void Render();
    
    static void RenderDemos();
    
public:
    
    static ImVec4& GetClearColor() { return mClearColor; }
    static float GetUIScale() { return mUIScale; }
    
private:
    
    static void RenderSettings();
    
private:
    
    inline static ImVec4 mClearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    inline static float mUIScale = 1.15f;
};
