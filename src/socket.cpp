#include "socket.h"

class Socket
{
private:
    const char *port;
    int socketfd, newSocketfd;

public:
    void Listen()
    {
        int portno;
        socklen_t clilen;
        char buffer[256];
        struct sockaddr_in serv_addr, cli_addr;
        int n;

        if (strlen(Socket::port) < 2)
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
        portno = atoi(Socket::port);
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
            const char *response = OnMessageReceived(buffer);
            int length = strlen(response);

            n = write(newSocketfd, response, length);
            if (n < 0)
            {
                std::cout << "Couldn't write to socket";
                return;
            }
        }
    }

    ~Socket()
    {
        close(newSocketfd);
        close(socketfd);
    }

    Socket(const char *port)
    {
        Socket::port = port;
    }
};