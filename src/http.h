#pragma once
#include <iostream>
#include <algorithm>
#include "hashtable.h"

class HTTP
{
    public:
        Hashtable properties;
        Hashtable parameters;
        std::string method;
        std::string body;

        HTTP(Hashtable properties, Hashtable parameters, std::string method = "GET", std::string body = "")
        {
            this->properties = properties;
            this->parameters = parameters;
            this->body = body;
            this->method = method;
        }
};

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

std::string ToLowerCase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

Hashtable GetHTTPProperties(std::string request)
{
    Hashtable properties;
    std::string property = "";
    std::string value = "";
    bool propertyFound = false;
    for (int i = 0; i < request.length(); i++)
    {
        if (request[i] == '\r' || request[i] == '\n') 
        {
            properties.Add(ToLowerCase(property), value);
            property = "";
            value = "";
            propertyFound = false;
            continue;
        }

        if (request[i] == ':' && !propertyFound)
        {
            propertyFound = true;
            continue;
        }

        if (!propertyFound && request[i] != ' ')
        {
            property += request[i];
        }
        
        if (propertyFound && (request[i] != ' ' || value.length() > 0))
        {
            value += request[i];
        }
    }
    return properties;
}

Hashtable GetHTTPParameters(std::string query)
{
    Hashtable parameters;
    std::string currentParameter = "";
    std::string currentValue = "";
    bool parameterFound = false;
    for (int i = 0; i < query.length(); i++)
    {
        if (query[i] == '=')
        {
            parameterFound = true;
            continue;
        }

        if (query[i] == '&' || i == query.length() - 1)
        {
            if (i == query.length() - 1)
            {
                currentValue += query[i];
            }
            parameters.Add(currentParameter, currentValue);
            currentParameter = "";
            currentValue = "";
            parameterFound = false;
            continue;
        }

        if (!parameterFound)
        {
            currentParameter += query[i];;
        }
        else
        {
            currentValue += query[i];
        }
    }
    return parameters;
}

HTTP GetHTTP(std::string request)
{
    std::string line = "";
    std::string method = "";
    std::string parameters = "";
    bool methodFound = false;
    bool parametersFound = false;
    Hashtable requestProperties = GetHTTPProperties(request);
    for (int i = 0; i < request.length(); i++)
    {
        if (request[i] == ' ' && !methodFound)
        {
            methodFound = true;
            continue;
        }
        
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

        if (!methodFound) {
            method += request[i];
        }
    }

    if (method == "POST")
    {
        int contentLength = atoi(requestProperties.Get("content-length").c_str());
        std::string body = request.substr(request.length() - contentLength, contentLength);
        return HTTP(requestProperties, GetHTTPParameters(parameters), method, body);
    }

    return HTTP(requestProperties, GetHTTPParameters(parameters), method);
}

std::string GetHTTPResponse(HTTPResponseCode code, std::string contentType, std::string body = "")
{
    std::string response = "HTTP/1.1 " + std::to_string((int)code) + " OK\r\ncontent-type: " + contentType + "\r\ncontent-length: " + std::to_string(body.length()) + "\r\n\r\n" + body;
    return response;
}