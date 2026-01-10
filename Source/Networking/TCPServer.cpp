#include "TCPServer.h"

#include "../Logging/Logger.h"

void TCPServer::Init()
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    const int wsaerr = WSAStartup(wVersionRequested, &wsaData);
    
    // WSAStartup returns 0 if it is successful or non-zero if failed
    if (wsaerr != 0)
    {
        Logger::Error("The Winsock dll not found!");
    }
    
    mSocket = INVALID_SOCKET;
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    if (mSocket == INVALID_SOCKET)
    {
        Logger::Error("Could not create socket", std::to_string(WSAGetLastError()));
    }
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(mListenAddress.c_str());
    server_addr.sin_port = htons(static_cast<u_short>(mListenPort));
    
    if (bind(mSocket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == SOCKET_ERROR)
    {
        Logger::Error("Bind failed", std::to_string(WSAGetLastError()));
        Shutdown();
    }
    
    Logger::Info("TCP Server initialized and listening on " + mListenAddress + ":" + std::to_string(mListenPort));
}

void TCPServer::Poll()
{
}

void TCPServer::Shutdown()
{
    closesocket(mSocket);
    mSocket = INVALID_SOCKET;
    
    WSACleanup();
    
    Logger::Info("TCP Server shutdown");
}
