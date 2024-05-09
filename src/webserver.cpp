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
    std::vector<std::string> files = File().GetFilesInDirectory(WEBSERVER_PATH);
    httplib::Server server;

    server.Get("/", [](const httplib::Request &req, httplib::Response &res)
               { res.set_content(WebServer::GetDefaultTemplate(), "text/html"); });

    for (int i = 0; i < files.size(); i++)
    {
        server.Get("/" + files[i], [files, i](const httplib::Request &req, httplib::Response &res)
                   { res.set_content(File().GetFileAsPlainText(std::string(WEBSERVER_PATH) + "/" + files[i]), WebServer::GetMimeType(files[i])); });
    }

    server.listen(path, port);
}

std::string WebServer::GetMimeType(std::string path)
{
    std::string temp = "";
    for (int i = path.length() - 1; i >= 0; i--)
    {
        if (path[i] == '.')
            break;
        temp.insert(0, 1, path[i]);
    }

    if (temp == "html")
        return "text/html";
    else if (temp == "css")
        return "text/css";
    else if (temp == "js")
        return "text/javascript";
    else
        return "text/plain";
}

std::string WebServer::GetDefaultTemplate()
{
    return File().GetFileAsPlainText(WEBSERVER_TEMPLATE_PATH);
}