#include "RocketGameControl.h"

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.cpp>
#include <sstream>

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
    
    ImGui::TextColored(TCPClient::GetStatusColor(), TCPClient::GetStatusStr().c_str());
    
    ImGui::End();
}

void RocketGameControl::RenderControls()
{
    ImGui::Begin("Game Control");
    
    // ImGui::Text("Control stuff for the game goes here");
    
    if (ImGui::BeginTabBar("Tabs"))
    {
        if (ImGui::BeginTabItem("Actors"))
        {
            RenderTabActors();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Lives"))
        {
            RenderTabLives();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Scenes"))
        {
            RenderTabScenes();
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Time"))
        {
            RenderTabTime();
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

void RocketGameControl::RenderTabActors()
{
    // spawn dropdown
    static const char* items[] = { "[E] Bomb Seeker", "[E] Bullet Bomber", "[E] Mine", "[E] Severance Drone", "[E] Wavy Rider" }; // FIXME: Make this a dictionary
    static const char* ids[] = { "BombSeeker", "BulletBomber", "Mine", "SeveranceDrone", "WavyRider" };                           // FIXME: Make this a dictionary
    static int current_item_index = 0;

    ImGui::Combo("Actor", &current_item_index, items, IM_ARRAYSIZE(items));
    
    // position input
    static float position[2] = { 300.0f, 150.0f };
    ImGui::InputFloat2("Position", position);
    
    // spawn button
    if (ImGui::Button("Spawn"))
    {
        std::stringstream ss;
        ss << "spawn_actor;" << ids[current_item_index] << ";[" << static_cast<int>(position[0]) << ", " << static_cast<int>(position[1]) << "]";
        TCPClient::SendData(ss.str());
    }
}

void RocketGameControl::RenderTabLives()
{
    // Lives
    static int lifeCount = 0;
    ImGui::InputInt("Life", &lifeCount);
    
    if (ImGui::Button("Add Life"))
    {
        std::stringstream ss;
        ss << "life_add;" << lifeCount;
        TCPClient::SendData(ss.str());
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove Life"))
    {
        std::stringstream ss;
        ss << "life_remove;" << lifeCount;
        TCPClient::SendData(ss.str());
    }
    
    // Life Slots
    static int lifeSlotCount = 0;
    ImGui::InputInt("Life Slot", &lifeSlotCount);
    
    if (ImGui::Button("Add Life Slot"))
    {
        std::stringstream ss;
        ss << "life_slot_add;" << lifeSlotCount;
        TCPClient::SendData(ss.str());
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove Life Slot"))
    {
        std::stringstream ss;
        ss << "life_slot_remove;" << lifeSlotCount;
        TCPClient::SendData(ss.str());
    }
}

void RocketGameControl::RenderTabScenes()
{
    static const char* ids[] = { "MainMenu", "Game", "GameOver" };
    static int currentIdx = 0;

    ImGui::Combo("Scene", &currentIdx, ids, IM_ARRAYSIZE(ids));
    
    if (ImGui::Button("Change"))
    {
        std::stringstream ss;
        ss << "scene_change;" << ids[currentIdx];
        TCPClient::SendData(ss.str());
    }
}

void RocketGameControl::RenderTabTime()
{
    ImGui::Text("Time control coming soon...");
}
