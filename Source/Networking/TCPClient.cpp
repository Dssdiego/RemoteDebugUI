#include "TCPClient.h"

#include "../Logging/Logger.h"

#include <iostream>

void TCPClient::Init()
{
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 2);

    const int wsaerr = WSAStartup(wVersionRequested, &wsaData);

    // WSAStartup returns 0 if it is successful or non-zero if failed
    if (wsaerr != 0)
    {
        Logger::Error("The Winsock dll not found!");
    }

    // socket creation
    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSocket == INVALID_SOCKET)
    {
        Logger::Error("Could not create socket", std::to_string(WSAGetLastError()));
    }

    // connect to server
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(mServerAddress.c_str());
    address.sin_port = htons(mServerPort);

    if (connect(mSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
    {
        Logger::Error("Connection to server failed", std::to_string(WSAGetLastError()));
        Shutdown();
    }

    Logger::Info("TCP Client initialized");

}

void TCPClient::Poll()
{
    //sending data
    // int sbyteCount = send(mSocket, "Ping", 200, 0);
    // if (sbyteCount == SOCKET_ERROR)
    // {
    //     std::cout << "Server send error: " << WSAGetLastError() << '\n';
    // }
    // else
    // {
    //     std::cout << "Server: sent" << sbyteCount << '\n';
    // }
}

void TCPClient::Shutdown()
{
    closesocket(mSocket);
    mSocket = INVALID_SOCKET;

    WSACleanup();

    Logger::Info("TCP Client shutdown");
}

void TCPClient::SendData(const std::string& data)
{
    int sbyteCount = send(mSocket, data.c_str(), static_cast<int>(strlen(data.c_str())), 0);
}
