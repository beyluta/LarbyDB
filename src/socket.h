#pragma once
#include <iostream>
#include <functional>

class Socket
{
public:
    const char *port;
    int serverSocketfd;
    void Listen();
    void SetPort(const char *port);
    std::function<std::string(const char *, const char *)> OnMessageReceived;
};
