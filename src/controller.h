#pragma once
#include "hashtable.h"
#include "packet.h"

// where the authentication key is stored in the db
#define DB_KEY_POSITION "K"

class Controller
{
private:
    bool IsStringANumber(string &str);

public:
    std::vector<Hashtable> hashtables;
    std::vector<Packet> packets;
    std::string tempIP;
    bool protectedByKey;
    void SetSize(int numTables = 2);
    int GetSize();
    string Get(string key, int table = 0);
    string GetResolvedResponse(string request);
    void GenerateKey(int length = 16);
    bool IsAuthorized();
};
