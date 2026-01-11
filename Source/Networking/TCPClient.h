#pragma once

#include <WinSock2.h>
#include <string>

class TCPClient
{
public:
    
    static void Init();
    static void Poll();
    static void Shutdown();
    
    static void SendData(const std::string& data);
    
private:
    
    inline static SOCKET mSocket = INVALID_SOCKET;
    
    inline static std::string mServerAddress = "127.0.0.1";
    inline static uint32_t mServerPort = 7241;
};
