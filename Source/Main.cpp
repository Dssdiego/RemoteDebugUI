#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

int main(int, char**)
{
    Renderer::SetScale(1.15f);
    Window::Init();
    
    while (!Window::ShouldClose())
    {
        Window::Update();
        Renderer::Render();
    }
    
    Renderer::Shutdown();
    Window::Shutdown();
}
