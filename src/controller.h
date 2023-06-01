#pragma once
#include "hashtable.h"
#include "packet.h"
#include "file.h"
#include "socket.h"
#include "logsys.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define DB_KEY_POSITION "K"

class Controller
{
private:
    bool IsStringANumber(std::string &str);

public:
    std::vector<Hashtable> hashtables;
    std::vector<Packet> packets;
    std::string tempIP;
    bool protectedByKey;
    void SetSize(int numTables = 2);
    int GetSize();
    std::string Get(std::string key, int table = 0);
    std::string GetAll(int table = 0);
    std::string GetResolvedResponse(std::string request);
    int Set(std::string key, std::string value, int table = 0);
    int Delete(std::string key, int table = 0);
    int Auth(std::string key);
    void GenerateKey(int length = 16);
    bool IsAuthorized();
};
