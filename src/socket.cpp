#include "socket.h"
using namespace SocketUtility;

const char *port;
int socketfd, newSocketfd;

void Socket::Listen()
{
    int portno;
    socklen_t clilen;
    char buffer[256];
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
        if (newSocketfd < 0)
        {
            std::cout << "Couldn't accept request" << std::endl;
            return;
        }

        bzero(buffer, 256);
        n = read(newSocketfd, buffer, 255);
        if (n < 0)
        {
            std::cout << "Couldn't read from socket" << std::endl;
            return;
        }

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

Socket::~Socket()
{
    close(newSocketfd);
    close(socketfd);
}

void Socket::SetPort(const char *portStr)
{
    port = portStr;
}
