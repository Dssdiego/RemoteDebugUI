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

    Logger::Info("TCP Client initialized");
}

void TCPClient::ConnectToServer()
{
    // if the socket is invalid when trying to connect to a server, initialize it first
    if (mSocket == INVALID_SOCKET)
    {
        Init();
    }
    
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(mServerAddress.c_str());
    address.sin_port = htons(mServerPort);
    
    Logger::Info("Trying to connect to server " + mServerAddress + ":" + std::to_string(mServerPort));

    if (connect(mSocket, reinterpret_cast<sockaddr*>(&address), sizeof(address)) == SOCKET_ERROR)
    {
        mStatus = EClientStatus::DISCONNECTED;
        Logger::Error("Connection to server failed", std::to_string(WSAGetLastError()));
    }
    else
    {
        mStatus = EClientStatus::CONNECTED;
    }
}

void TCPClient::DisconnectFromServer()
{
    Shutdown();
}

void TCPClient::SendData(const std::string& data)
{
    int sbyteCount = send(mSocket, data.c_str(), static_cast<int>(strlen(data.c_str())), 0);
}

void TCPClient::Shutdown()
{
    mStatus = EClientStatus::DISCONNECTED;
    
    closesocket(mSocket);
    mSocket = INVALID_SOCKET;

    WSACleanup();

    Logger::Info("TCP Client shutdown");
}

std::string TCPClient::GetStatusStr()
{
    switch (mStatus)
    {
        case EClientStatus::CONNECTED:
            return "Status: Connected";
        case EClientStatus::DISCONNECTED:
            return "Status: Disconnected";
    }
    
    return "Status: -";
}

std::string TCPClient::GetStatusStrForConnectButton()
{
    switch (mStatus)
    {
        case EClientStatus::CONNECTED:
            return "Disconnect";
        case EClientStatus::DISCONNECTED:
            return "Connect";
    }
    
    return "-";
}
