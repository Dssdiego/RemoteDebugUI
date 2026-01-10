#pragma once

#include <WinSock2.h>
#include <string>

class TCPServer
{
public:
    
    static void Init();
    static void Poll();
    static void Shutdown();
    
public:
    
    static std::string GetListenPortString() { return std::to_string(mListenPort); }
    
private:
 
    inline static std::string mListenAddress = "127.0.0.1";
    inline static uint32_t mListenPort = 9200;
    
    inline static SOCKET mSocket = INVALID_SOCKET;
};
