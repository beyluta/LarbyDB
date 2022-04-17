#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "hashtable.h"
#include "file.h"
#include "socket.h"
#include "packet.h"

using namespace HashtableUtility;

// where the authentication key is stored in the db
#define DB_KEY_POSITION "K"

namespace ControllerUtility
{
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
        string GetResolvedResponse(string request);
        void GenerateKey(int length = 16);
        bool IsAuthorized();
    };
}