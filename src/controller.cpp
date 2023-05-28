#include "controller.h"
#include "file.h"
#include "socket.h"
#include "logsys.h"
#include <iostream>
#include <string>
#include <vector>

bool Controller::IsStringANumber(string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

std::vector<Hashtable> hashtables;
std::vector<Packet> packets;
std::string tempIP;
bool protectedByKey = false;

void Controller::SetSize(int numTables)
{
    if (numTables < 2)
    {
        numTables = 2;
    }

    hashtables.resize(numTables + 1);
}

int Controller::GetSize()
{
    return hashtables.size();
}

string Controller::Get(string key, int table)
{
    if (table < 0 || table >= hashtables.size() || !IsAuthorized())
    {
        return "";
    }

    return hashtables[table].Get(key);
}

string Controller::GetAll(int table)
{
    if (table < 0 || table >= hashtables.size() || !IsAuthorized())
    {
        return "";
    }

    return hashtables[table].GetAll();
}

int Controller::Set(string key, string value, int table)
{
    if (table < 0 || table >= hashtables.size() || !IsAuthorized())
    {
        return 1;
    }

    value.erase(std::remove(value.begin(), value.end(), '\n'), value.cend());
    hashtables[table].Add(key, value);
    return 0;
}

int Controller::Delete(string key, int table)
{
    if (table < 0 || table >= hashtables.size() || !IsAuthorized())
    {
        return 1;
    }

    if (IsStringANumber(key))
    {
        hashtables[table].Remove(atoi(key.c_str()));
        return 0;
    }

    hashtables[table].Remove(key);
    return 0;
}

int Controller::Auth(string key)
{
    if (hashtables[0].Get(DB_KEY_POSITION) == key)
    {
        hashtables[0].Remove(tempIP);
        hashtables[0].Add(tempIP);
        Logsys::LogActivity(tempIP + " Authorized on this machine", Logsys::IOSystem::BOTH, Logsys::Color::BLUE);
        return 0;
    }

    return 1;
}

string Controller::GetResolvedResponse(string request)
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
            Logsys::LogActivity(tempIP + " Authorized on this machine", Logsys::IOSystem::BOTH, Logsys::Color::BLUE);
            return "OK";
        }
    }

    if (!IsAuthorized())
    {
        Logsys::LogActivity(tempIP + " Machine not authorized: type 'AUTH <KEY>' to authorize this machine", Logsys::IOSystem::BOTH, Logsys::Color::RED);
        return "Unauthorized";
    }

    if (words[0].find("SET") != string::npos && words.size() > 3)
    {
        string &key = words[1];
        key.pop_back();
        int table = atoi(words[2].c_str());

        if (table <= 0)
        {
            Logsys::LogActivity("Attempt to write to table " + to_string(table) + " failed: Does not exist, is out of bounds, or forbidden to write to", Logsys::IOSystem::BOTH, Logsys::Color::RED);
            return "Forbidden";
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

            Logsys::LogActivity("Key: " + key + "; Value: " + value + "; has been added to the database", Logsys::IOSystem::BOTH, Logsys::Color::BLUE);
            return "Created";
        }
    }

    if (words[0].find("GET") != string::npos && words.size() > 2)
    {
        string &key = words[1];
        key.pop_back();
        int table = atoi(words[2].c_str());

        if (table <= 0 || table >= hashtables.size())
        {
            Logsys::LogActivity("Attempt to read from table " + to_string(table) + " failed: Does not exist, is out of bounds, or forbidden to read from", Logsys::IOSystem::BOTH, Logsys::Color::RED);
            return "Forbidden";
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
                    Logsys::LogActivity("Retrieved from table " + to_string(table) + " values rangin from " + to_string(start) + " to " + to_string(end), Logsys::IOSystem::BOTH, Logsys::Color::BLUE);
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
                        Logsys::LogActivity("Retrieved " + to_string(amount) + " values starting from " + to_string(skip), Logsys::IOSystem::BOTH, Logsys::Color::BLUE);
                        return hashtables[table].GetAmount(amount, skip);
                    }
                }
            }
            else
            {
                Logsys::LogActivity("Retrieved all values", Logsys::IOSystem::BOTH, Logsys::Color::BLUE);
                return hashtables[table].GetAll();
            }
        }
        else
        {
            Logsys::LogActivity("Value of key: " + key + "; does not exist", Logsys::IOSystem::BOTH, Logsys::Color::RED);
            return hashtables[table].Get(key) == "" ? "Not Found" : hashtables[table].Get(key);
        }
    }

    if (words[0].find("DEL") != string::npos && words.size() > 2)
    {
        string &key = words[1];
        key.pop_back();
        int table = atoi(words[2].c_str());

        if (table <= 0)
        {
            Logsys::LogActivity("Attempt to delete from table " + to_string(table) + " failed: Does not exist, is out of bounds, or forbidden to delete from", Logsys::IOSystem::BOTH, Logsys::Color::RED);
            return "Forbidden";
        }

        if (IsStringANumber(key))
        {
            hashtables[table].Remove(atoi(key.c_str()));
            return "OK";
        }

        Logsys::LogActivity("Value of key: " + key + "; has been deleted", Logsys::IOSystem::BOTH, Logsys::Color::BLUE);
        hashtables[table].Remove(key);
        return "OK";
    }
    return "Unauthorized";
}

void Controller::GenerateKey(int length)
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

bool Controller::IsAuthorized()
{
    return hashtables[0].Contains(tempIP) || !protectedByKey ? true : false;
}
