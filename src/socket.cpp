#include "socket.h"
#include "logsys.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#ifdef __unix__
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void Socket::Listen()
{
    int portno;
    socklen_t clilen;
    char buffer[MAX_BUFFER_SIZE];
    sockaddr_in serv_addr, cli_addr;
    int n;

    if (strlen(port) < 2)
    {
        Logsys::LogActivity("Invalid port number.", Logsys::IOSystem::BOTH, Logsys::Color::RED);
        return;
    }

    serverSocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketfd < 0)
    {
        Logsys::LogActivity("Error opening socket.", Logsys::IOSystem::BOTH, Logsys::Color::RED);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(serverSocketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        Logsys::LogActivity("Error on binding.", Logsys::IOSystem::BOTH, Logsys::Color::RED);
        return;
    }

    if (listen(serverSocketfd, MAX_CONN) < 0)
    {
        Logsys::LogActivity("Error on listening.", Logsys::IOSystem::BOTH, Logsys::Color::RED);
    }

    fd_set sockets, ready_sockets;
    FD_ZERO(&sockets);
    FD_SET(serverSocketfd, &sockets);
    timeval timeout;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    while (true)
    {
        ready_sockets = sockets;

        if (select(FD_SETSIZE, &ready_sockets, nullptr, nullptr, &timeout) < 0)
        {
            Logsys::LogActivity("Error on select.", Logsys::IOSystem::BOTH, Logsys::Color::RED);
            FD_ZERO(&sockets);
        }

        for (int i = 0; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if (i == serverSocketfd)
                {
                    int clientSocket = accept(serverSocketfd, (struct sockaddr *)&cli_addr, &clilen);

                    if (clientSocket < 0)
                    {
                        Logsys::LogActivity("Error on accepting.", Logsys::IOSystem::BOTH, Logsys::Color::RED);
                        close(i);
                        break;
                    }
                    else
                    {
                        char *ipAddr = inet_ntoa(cli_addr.sin_addr);
                        FD_SET(clientSocket, &sockets);
                    }
                }
                else
                {
                    bzero(buffer, MAX_BUFFER_SIZE);
                    n = recv(i, buffer, MAX_BUFFER_SIZE - 1, MSG_DONTWAIT);
                    char *ipAddr = inet_ntoa(cli_addr.sin_addr);

                    if (n >= MAX_BUFFER_SIZE)
                    {
                        Logsys::LogActivity("Received message from " + std::string(ipAddr) + " is too large.", Logsys::IOSystem::BOTH, Logsys::Color::RED);
                        close(i);
                        Logsys::LogActivity("Disconnected " + std::string(ipAddr), Logsys::IOSystem::BOTH, Logsys::Color::GREEN);
                    }
                    else if (n == 0)
                    {
                        Logsys::LogActivity("Disconnected " + std::string(ipAddr), Logsys::IOSystem::BOTH, Logsys::Color::GREEN);
                    }
                    else
                    {
                        std::string response = OnMessageReceived(buffer, ipAddr);
                        int length = strlen(response.c_str());
                        send(i, response.c_str(), length, 0);
                        close(i);
                    }

                    FD_CLR(i, &sockets);
                }
            }
        }
    }
}

void Socket::PortSet(const char *portStr)
{
    port = portStr;
}
#elif _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

void Socket::Listen()
{
    WSADATA wsaData;
    SOCKADDR_IN serverAddr, clientAddr;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
    const auto server = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(Socket::port));
    ::bind(server, reinterpret_cast<SOCKADDR *>(&serverAddr), sizeof(serverAddr));
    listen(server, 0);
    int clientAddrSize = sizeof(clientAddr);

    for (;;) {
        SOCKET client;

        if ((client = accept(server, reinterpret_cast<SOCKADDR *>(&clientAddr), &clientAddrSize)) != INVALID_SOCKET) {
            char buffer[1024];
            recv(client, buffer, sizeof(buffer), 0);
            char *ipAddr = inet_ntoa(clientAddr.sin_addr);
            std::string response = OnMessageReceived(buffer, ipAddr);
            int length = strlen(response.c_str());
            send(client, response.c_str(), length, 0);
            closesocket(client);
        }
    }
}

void Socket::PortSet(const char *portStr)
{
    Socket::port = portStr;
}
#endif
