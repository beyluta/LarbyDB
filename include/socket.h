#pragma once
#include <iostream>
#include <functional>

#define MAX_BUFFER_SIZE 1048576
#define MAX_CONN 4

class Socket
{
public:
    const char *port;
    int serverSocketfd;
    void Listen();
    void PortSet(const char *portStr);
    std::function<std::string(const char *, const char *)> OnMessageReceived;
};
