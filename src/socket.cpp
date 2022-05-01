#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1048576

const char *port;
int socketfd, newSocketfd;

void Socket::Listen()
{
    int portno;
    socklen_t clilen;
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (strlen(port) < 2)
    {
        std::cout << "Invalid Port" << std::endl;
        return;
    }

    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0)
    {
        std::cout << "Couldn't Open Port" << std::endl;
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(port);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "Couldn't bind to server address" << std::endl;
        return;
    }

    for (;;)
    {
        listen(socketfd, 5);
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
            n = read(newSocketfd, buffer, MAX_BUFFER_SIZE - 1);

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
    }
}

Socket::~Socket()
{
    close(newSocketfd);
    close(socketfd);
}

void Socket::SetPort(const char *portStr)
{
    port = portStr;
}
