#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1048576

const char *port;
int serverSocketfd;

void Socket::Listen()
{
    int portno;
    socklen_t clilen;
    char buffer[MAX_BUFFER_SIZE];
    sockaddr_in serv_addr, cli_addr;
    int n;

    if (strlen(port) < 2)
    {
        std::cout << "Invalid Port" << std::endl;
        return;
    }

    serverSocketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocketfd < 0)
    {
        std::cout << "Couldn't Open Port" << std::endl;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(serverSocketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Couldn't bind to server address" << std::endl;
        return;
    }
    #define MAX_CONN 4
    if (listen(serverSocketfd, MAX_CONN) < 0)
    {
        std::cout << "Error occured while listening\n";
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
            std::cout << "Error in select()\n";
            FD_ZERO(&sockets);
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if (i == serverSocketfd)
                {
                    int clientSocket = accept(serverSocketfd, (struct sockaddr *)&cli_addr, &clilen);
                    if (clientSocket < 0)
                    {
                        std::cout << "Failed to accept client\n";
                    }
                    else
                    {
                        char *ipAddr = inet_ntoa(cli_addr.sin_addr);
                        std::cout << ipAddr << " connected\n";
                        FD_SET(clientSocket, &sockets);
                    }
                }
                else
                {
                    bzero(buffer, MAX_BUFFER_SIZE);
                    n = recv(i, buffer, MAX_BUFFER_SIZE - 1, MSG_DONTWAIT); // MSG_DONTWAIT
                    char *ipAddr = inet_ntoa(cli_addr.sin_addr);
                    if (n >= MAX_BUFFER_SIZE)
                    {
                        std::cout << "Message exceeded buffer limit" << std::endl;
                        close(i);
                        std::cout << ipAddr << " disconnected\n";
                    }
                    else if (n == 0)
                    {
                        std::cout << ipAddr << " disconnected\n";
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

        /*
    for (;;)
    {
        
        clilen = sizeof(cli_addr);
        newSocketfd = accept(socketfd, (struct sockaddr *)&cli_addr, &clilen);
        

        
        bool isMessageValid = true;

        if (newSocketfd < 0)
        {
            std::cout << "Couldn't accept request" << std::endl;
            isMessageValid = false;
        }

        if (isMessageValid)
        {
            bzero(buffer, MAX_BUFFER_SIZE);
            n = recv(newSocketfd, buffer, MAX_BUFFER_SIZE - 1, MSG_DONTWAIT);
            //n = read(newSocketfd, buffer, MAX_BUFFER_SIZE - 1);

            if (n >= MAX_BUFFER_SIZE - 1)
            {
                std::cout << "Message exceeded buffer limit" << std::endl;
                isMessageValid = false;
            }

            if (n < 0)
            {
                std::cout << "Couldn't read from socket" << std::endl;
                isMessageValid = false;
            }
        }

        if (isMessageValid)
        {
            char *ipAddr = inet_ntoa(cli_addr.sin_addr);
            std::string response = OnMessageReceived(buffer, ipAddr);
            int length = strlen(response.c_str());

            n = write(newSocketfd, response.c_str(), length);
            if (n < 0)
            {
                std::cout << "Couldn't write to socket";
            }
        }
        close(newSocketfd);
        
    }
        */
}

void Socket::SetPort(const char *portStr)
{
    port = portStr;
}
