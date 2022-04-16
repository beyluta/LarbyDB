#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <functional>

namespace SocketUtility
{
    class Socket
    {
    public:
        void Listen();
        ~Socket();
        void SetPort(const char *port);
        std::function<std::string(const char *, const char *)> OnMessageReceived;
    };
}