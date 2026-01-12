#pragma once

class RocketGameControl
{
public:
    
    static void Render();
    
private:
    
    static void RenderServerConnection();
    static void RenderControls();
    
    static void RenderTabActors();
    static void RenderTabTime();
};
