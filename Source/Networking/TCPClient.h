#pragma once

#include <WinSock2.h>
#include <string>

enum class EClientStatus : uint8_t
{
    CONNECTED,
    DISCONNECTED,
};

class TCPClient
{
public:
    
    static void Init();
    
    static void ConnectToServer();
    static void SendData(const std::string& data);
    
    static void Shutdown();
    
public:
    
    static std::string GetStatusStr();
    
    static std::string GetAddress() { return mServerAddress; }
    static uint32_t GetPort() { return mServerPort; }
    static std::string GetPortStr() { return std::to_string(mServerPort); }
    
private:
    
    inline static SOCKET mSocket = INVALID_SOCKET;
    
    inline static std::string mServerAddress = "127.0.0.1";
    inline static uint32_t mServerPort = 7241;
    
    inline static EClientStatus mStatus = EClientStatus::DISCONNECTED;
};
