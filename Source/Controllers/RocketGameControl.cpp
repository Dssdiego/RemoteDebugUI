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
    if (ImGui::InputText("IP", &serverAddress))
    {
        TCPClient::SetAddress(serverAddress);
    }
    
    static std::string serverPort = TCPClient::GetPortStr();
    if (ImGui::InputText("Port", &serverPort))
    {
        TCPClient::SetPortNumber(std::stoul(serverPort));
    }
    
    // conection button
    if (ImGui::Button(TCPClient::GetStatusStrForConnectButton().c_str()))
    {
        auto status = TCPClient::GetStatus();

        switch (status)
        {
            case EClientStatus::CONNECTED:
            {
                TCPClient::DisconnectFromServer();
                ImGui::End();
                return;
            }
            case EClientStatus::DISCONNECTED:
            {
                TCPClient::ConnectToServer();
                ImGui::End();
                return;
            }
        }
    }
    
    ImGui::Text(TCPClient::GetStatusStr().c_str());
    
    ImGui::End();
}

void RocketGameControl::RenderControls()
{
    ImGui::Begin("Controls");
    
    ImGui::Text("Control stuff for the game goes here");
    
    if (ImGui::Button("Test Enemy Spawn"))
    {
        TCPClient::SendData("debug_spawn_enemy");
    }
    
    ImGui::End();
}
