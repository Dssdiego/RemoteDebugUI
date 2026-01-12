#include "RocketGameControl.h"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.cpp>

#include "../Logging/Logger.h"
#include "../Networking/TCPClient.h"

void RocketGameControl::Render()
{
    RenderServerConnection();
    RenderControls();
}

void RocketGameControl::RenderServerConnection()
{
    ImGui::Begin("Server (Game) Connection");
    
    static std::string serverAddress = TCPClient::GetAddress();
    ImGui::InputText("IP", &serverAddress);
    
    static std::string serverPort = TCPClient::GetPortStr();
    ImGui::InputText("Port", &serverPort);
    
    if (ImGui::Button("Connect"))
    {
        TCPClient::ConnectToServer();
    }
    
    ImGui::Text(TCPClient::GetStatusStr().c_str());
    
    // if (ImGui::Button("Click me"))
    // {
    //     Logger::Info("Button was clicked in Debug UI");
    //     TCPClient::SendData("Hello from Debug UI!");
    // }
    
    ImGui::End();
}

void RocketGameControl::RenderControls()
{
    ImGui::Begin("Controls");
    
    ImGui::Text("Control stuff for the game goes here");
    
    ImGui::End();
}
