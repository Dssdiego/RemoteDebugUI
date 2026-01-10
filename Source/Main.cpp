#include "Rendering/Renderer.h"
#include "Rendering/Window.h"

#include "Networking/TCPServer.h"

int main(int, char**)
{
    Renderer::SetScale(1.15f);
    Window::Init();
    
    TCPServer::Init();
    
    while (!Window::ShouldClose())
    {
        TCPServer::Poll();
        
        Window::Update();
        Renderer::Render();
    }
    
    TCPServer::Shutdown();
    
    Renderer::Shutdown();
    Window::Shutdown();
}
