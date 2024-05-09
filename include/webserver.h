#pragma once
#include <iostream>
#include <string>
#include <thread>
#include "httplib.h"
#include "file.h"

#define WEBSERVER_TEMPLATE_PATH "templates/index.html"

class WebServer
{
public:
    WebServer(std::string path, int port);

private:
    void Initialize(std::string path, int port);
    static std::string GetDefaultTemplate();
};