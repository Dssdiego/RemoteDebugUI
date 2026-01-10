#include "RocketGameControl.h"

#include <imgui/imgui.h>
#include "../Logging/Logger.h"

void RocketGameControl::Render()
{
    RenderTCP();
    RenderControls();
}

void RocketGameControl::RenderTCP()
{
    ImGui::Begin("TCP Server");
    
    static char bfIP[128] = "127.0.0.1";
    ImGui::InputText("IP", bfIP, sizeof(bfIP));
    
    ImGui::Text("Something nice");
    
    if (ImGui::Button("Click me"))
    {
        Logger::Info("Button was clicked in Debug UI");
    }
    
    ImGui::End();
}

void RocketGameControl::RenderControls()
{
    ImGui::Begin("Controls");
    
    ImGui::Text("Control stuff for the game goes here");
    
    ImGui::End();
}
