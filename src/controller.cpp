#include "controller.h"
class Controller
{
private:
    bool IsStringANumber(string &str)
    {
        return std::all_of(str.begin(), str.end(), ::isdigit);
    }

public:
    std::vector<Hashtable> hashtables;
    std::vector<Packet> packets;
    std::string tempIP;
    bool protectedByKey = false;

    void SetSize(int numTables = 2)
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

    string GetResolvedResponse(string request)
    {
        vector<string> words;
        string word;

        for (int i = 0; i < request.size(); i++)
        {
            word += request[i];

            if (request[i] == ' ' || request.size() - 1 == i)
            {
                words.push_back(std::move(word));
            }
        }

        words.back().pop_back();

        if (words[0].find("AUTH") != string::npos && words.size() > 1)
        {
            if (hashtables[0].Get(DB_KEY_POSITION) == words[1])
            {
                hashtables[0].Add(tempIP);
                return GetHttpStatusCode(200);
            }
        }

        if (!IsAuthorized())
        {
            return GetHttpStatusCode(401);
        }

        if (words[0].find("SET") != string::npos && words.size() > 3)
        {
            string &key = words[1];
            key.pop_back();
            int table = atoi(words[2].c_str());

            if (table <= 0)
            {
                return GetHttpStatusCode(403);
            }

            int ttl = atoi(words[3].c_str());
            string value;

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
            key.pop_back();
            int table = atoi(words[2].c_str());

            if (table <= 0 || table >= hashtables.size())
            {
                return GetHttpStatusCode(403);
            }

            if (key.find("ALL") != string::npos)
            {
                if (words.size() > 3)
                {
                    int dash = words[3].find("-");

                    if (dash != string::npos)
                    {
                        int start = atoi((words[3].substr(0, dash)).c_str());
                        int end = atoi(words[3].substr(dash + 1).c_str());
                        return hashtables[table].GetInRange(start, end);
                    }
                    else
                    {
                        int amount = atoi(words[3].c_str());
                        int skip = 1;

                        if (words.size() > 4)
                        {
                            skip = atoi(words[4].c_str());
                        }

                        if (amount > 0 && skip > 0)
                        {
                            return hashtables[table].GetAmount(amount, skip);
                        }
                    }
                }
                else
                {
                    return hashtables[table].GetAll();
                }
            }
            else
            {
                return hashtables[table].Get(key) == "" ? GetHttpStatusCode(404) : hashtables[table].Get(key);
            }
        }

        if (words[0].find("DEL") != string::npos && words.size() > 2)
        {
            string &key = words[1];
            key.pop_back();
            int table = atoi(words[2].c_str());

            if (table <= 0)
            {
                return GetHttpStatusCode(403);
            }

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

    void GenerateKey(int length = 16)
    {
        srand(time(nullptr));
        string key;

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

        hashtables[0].Remove(hashtables[0].Hash(DB_KEY_POSITION));
        hashtables[0].Add(DB_KEY_POSITION, key);
    }

    bool IsAuthorized()
    {
        return hashtables[0].Contains(tempIP) || !protectedByKey ? true : false;
    }
};