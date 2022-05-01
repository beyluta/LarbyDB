#pragma once
#include <iostream>
#include <functional>

class Socket
{
public:
    void Listen();
    ~Socket();
    void SetPort(const char *port);
    std::function<std::string(const char *, const char *)> OnMessageReceived;
};
