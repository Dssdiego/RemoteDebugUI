#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

#include "Networking/TCPClient.h"

int main(int, char**)
{
    Window::Init();
    
    TCPClient::Init();
    
    while (!Window::ShouldClose())
    {
        Window::Update();
        Renderer::Render();
    }
    
    TCPClient::Shutdown();
    
    Renderer::Shutdown();
    Window::Shutdown();
}
