#include "controller.h"

class Controller
{
private:
    bool IsStringANumber(string &str)
    {
        return std::all_of(str.begin(), str.end(), ::isdigit); //fancy c++11 code
    }

public:
    std::vector<Hashtable> hashtables;
    std::vector<Packet> packets;
    std::string tempIP;
    bool protectedByKey = false;

    Controller(int numTables = 2)
    {
        if (numTables < 2)
        {
            numTables = 2;
        }
        hashtables.resize(numTables);
    }

    int GetSize()
    {
        return hashtables.size();
    }

    /*Function to interpret the input string. Spilts the string into words,
    then checks which commands to execute.*/
    string GetResolvedResponse(string request)
    {
        request += " "; // required for the check below
        vector<string> words;
        string word;
        for (int i = 0; i < request.size(); i++)
        {
            word += request[i];
            if (request[i] == ' ')
            {
                words.push_back(std::move(word));
            }
        }
        words.back().pop_back();
        words.back().pop_back();

        if (words[0].find("AUTH") != string::npos && words.size() > 1)
        {
            string &key = words[1];

            if (hashtables[0].Contains(key))
            {
                hashtables[0].Add(tempIP);
                return GetHttpStatusCode(200);
            }
        }

        if (!IsAuthorized())
            return GetHttpStatusCode(401);

        if (words[0].find("SET") != string::npos && words.size() > 3)
        {
            string &key = words[1];
            key.pop_back();

            int table = atoi(words[2].c_str());
            if (table <= 0)
                return GetHttpStatusCode(403); //0 shouldn't be accessible

            int ttl = atoi(words[3].c_str());
            string value = "";
            for (int i = 4; i < words.size(); i++)
            {
                value += words[i];
            }

            if (table >= hashtables.size())
            {
                hashtables.resize(table + 1);
            }

            if (table < hashtables.size())
            {
                int hash = hashtables[table].Add(key, value);
                if (ttl > 0)
                {
                    Packet p;
                    p.hash = hash;
                    p.time_to_live = ttl;
                    p.table = table;
                    packets.push_back(p);
                }
                return GetHttpStatusCode(201);
            }
        }

        if (words[0].find("GET") != string::npos && words.size() > 2)
        {
            string &key = words[1];
            int table = atoi(words[2].c_str());
            if (table <= 0)
                return GetHttpStatusCode(403);

            if (key.find("ALL") != string::npos) //change ALL to * later...
            {
                return hashtables[table].GetAll();
            }
            else
            {
                return hashtables[table].Get(key);
            }
        }

        if (words[0].find("DEL") != string::npos && words.size() > 2)
        {
            string &key = words[1];
            words[1].pop_back();
            int table = atoi(words[2].c_str());
            if (table <= 0)
                return GetHttpStatusCode(403);

            if (IsStringANumber(key))
            {
                hashtables[table].Remove(atoi(key.c_str()));
                return GetHttpStatusCode(200);
            }

            hashtables[table].Remove(key);
            return GetHttpStatusCode(200);
        }
        return GetHttpStatusCode(401);
    }

    std::string GenerateKey(int length = 16)
    {
        srand(time(nullptr));
        std::string key;
        for (int i = 0; i < length; i++)
        {
            switch (rand() % 3)
            {
            case (0):
                key += std::to_string(rand() % 10);
                break;
            case (1):
                key += ('A' + rand() % 26);
                break;
            case (2):
                key += ('a' + rand() % 26);
                break;
            }
        }
        std::string output = key;
        hashtables[0].Add(key);
        return output;
    }

    bool IsAuthorized()
    {
        if (hashtables[0].Contains(tempIP) || !protectedByKey)
            return true;
        else
            return false;
    }
};