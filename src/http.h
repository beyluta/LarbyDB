#pragma once
#include <iostream>
#include "hashtable.h"

enum class HTTPResponseCode
{
    OK = 200,
    CREATED = 201,
    ACCEPTED = 202,
    NO_CONTENT = 204,
    BAD_REQUEST = 400,
    UNAUTHORIZED = 401,
    FORBIDDEN = 403,
    NOT_FOUND = 404,
    METHOD_NOT_ALLOWED = 405,
    INTERNAL_SERVER_ERROR = 500,
    NOT_IMPLEMENTED = 501,
    BAD_GATEWAY = 502,
    SERVICE_UNAVAILABLE = 503,
    GATEWAY_TIMEOUT = 504
};

std::string PrepareHTTPResponse(std::string request, std::string response)
{
    // int length = response.length();
    // return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(length) + "\r\n\r\n" + response;

    std::string line = "";
    std::string method = "";
    std::string parameters = "";
    bool methodFound = false;
    bool parametersFound = false;
    for (int i = 0; i < request.length(); i++)
    {
        if (request[i] == ' ' && !methodFound)
        {
            methodFound = true;
            continue;
        }

        if (methodFound && method == "GET")
        {
            if (request[i] == '?' && !parametersFound)
            {
                parametersFound = true;
                continue;
            } 
            else if (request[i] == ' ' && parametersFound)
            {
                parametersFound = false;
                continue;
            }

            if (parametersFound) {
                parameters += request[i];
            }
        }

        if (request[i] == '\r' || request[i] == '\n') 
        {
            break;
        }

        if (!methodFound) {
            method += request[i];
        }
    }
    return request;
}