#include "webserver.h"
#include "logsys.h"

WebServer::WebServer(std::string path, int port)
{
    std::thread thread{&WebServer::Initialize, this, path, port};
    thread.detach();
    Logsys::LogActivity("Webserver for Interface initialized", Logsys::IOSystem::BOTH, Logsys::Color::GREEN);
}

void WebServer::Initialize(std::string path, int port)
{
    httplib::Server server;
    server.Get("/", [](const httplib::Request &req, httplib::Response &res)
               { res.set_content(WebServer::GetDefaultTemplate(), "text/html"); });
}

std::string WebServer::GetDefaultTemplate()
{
    return File().GetFileAsPlainText(WEBSERVER_TEMPLATE_PATH);
}