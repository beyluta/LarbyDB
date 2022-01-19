#include "controller.h"

class Controller
{
private:
    std::vector<std::string> GetStringValues(std::string str, char delimiter)
    {
        int occurence = 0;
        std::string word = "";
        std::vector<std::string> words;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == delimiter)
            {
                occurence++;

                if (occurence % 2 == 0)
                {
                    words.push_back(word);
                    word = "";
                }

                continue;
            }

            if (occurence % 2 != 0)
            {
                word += str[i];
            }
        }
        return words;
    }

    std::vector<std::string> GetStringCommands(std::string str, char delimiter)
    {
        int occurence = 0;
        std::string word = "";
        std::vector<std::string> words;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == delimiter)
            {
                occurence++;

                if (occurence % 2 != 0)
                {
                    words.push_back(word);
                    word = "";
                }

                continue;
            }

            if (occurence % 2 == 0)
            {
                word += str[i];
            }
        }
        return words;
    }

    bool VectorContains(std::vector<std::string> vector, std::string match)
    {
        for (int i = 0; i < vector.size(); i++)
        {
            if (vector[i].find(match) != std::string::npos)
            {
                return true;
            }
        }
        return false;
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

    /*Function to interpret the input string. Spilts the string into commands and words,
    then checks which commands to execute.*/
    std::string ResolveStringCommand(std::string command)
    {
        std::vector<std::string> words = GetStringValues(command, '/');
        std::vector<std::string> commands = GetStringCommands(command, '/');

        //TODO: Check for vulnerabilities. Maybe clean the code up a bit...
        if (VectorContains(commands, "AUTH"))
        {
            if (protectedByKey)
            {
                if (words.size() > 0 && words[0] != "")
                {
                    if (hashtables[0].Contains(words[0]))
                    {
                        if (!hashtables[0].Contains(tempIP))
                        {
                            hashtables[0].Add(tempIP);
                        }
                        return GetHttpStatusCode(200);
                    }
                }
                return GetHttpStatusCode(403);
            }
            return GetHttpStatusCode(403);
        }

        else if (protectedByKey == false || hashtables[0].Contains(tempIP))
        {
            if (VectorContains(commands, "INSERT"))
            {
                if (VectorContains(commands, "INTO") && words[1] != "")
                {
                    int index = atoi(words[1].c_str());
                    if (index < 1)
                        return GetHttpStatusCode(403);
                    if (VectorContains(commands, "WHEREKEY") && words.size() >= 3)
                    {
                        int ttl = 0;
                        if (VectorContains(commands, "TTL"))
                        {
                            ttl = atoi(words[3].c_str());
                        }

                        if (index >= hashtables.size())
                        {
                            hashtables.resize(index + 1);
                        }

                        if (index < hashtables.size())
                        {
                            int hash = hashtables[index].Add(words[2], words[0]);
                            if (ttl > 0)
                            {
                                Packet p;
                                p.hash = hash;
                                p.time_to_live = ttl;
                                p.table = index;
                                packets.push_back(p);
                            }
                            return GetHttpStatusCode(201);
                        }
                    }
                    else if (!VectorContains(commands, "WHEREKEY"))
                    {
                        if (index < hashtables.size())
                        {
                            hashtables[index].Add(words[0]);
                            return GetHttpStatusCode(201);
                        }
                    }
                }
            }

            else if (VectorContains(commands, "DELETE"))
            {
                if (VectorContains(commands, "FROM") && words.size() >= 2 && words[1] != "")
                {
                    int index = atoi(words[1].c_str());
                    if (index < 1)
                        return GetHttpStatusCode(403);

                    if (index < hashtables.size())
                    {
                        if (VectorContains(commands, "ASINT"))
                        {
                            hashtables[index].Remove(atoi(words[0].c_str()));
                            return GetHttpStatusCode(200);
                        }
                        else if (!VectorContains(commands, "ASINT"))
                        {
                            hashtables[index].Remove(words[0]);
                            return GetHttpStatusCode(200);
                        }
                    }
                }
            }

            /* TODO: Currently if the db is of {x} size and you try to fetch
               from the {x} table, it will throw a segmentation fault.*/
            else if (VectorContains(commands, "FETCH"))
            {
                if (VectorContains(commands, "FROM") && words.size() >= 2 && words[1] != "")
                {
                    int index = atoi(words[1].c_str());
                    if (index < 1)
                        return GetHttpStatusCode(403);

                    std::string result;
                    if (index < hashtables.size())
                    {
                        if (words[0].length() == 1 && words[0] == "*")
                        {
                            result = hashtables[index].GetAll();
                        }
                        else
                        {
                            result = hashtables[index].Get(words[0]);
                        }
                        if (result == "")
                        {
                            return GetHttpStatusCode(404);
                        }
                        return result;
                    }
                }
            }
            return GetHttpStatusCode(404);
        }
        return GetHttpStatusCode(401);
    }

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
                words.push_back(word);
                word = "";
            }
        }

        if (words[0].find("AUTH") != string::npos)
        {
            string key = words[1];
        }

        if (words[0].find("SET") != string::npos)
        {
            string key = words[1];
            int table = atoi(words[2].c_str());
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

        if (words[0].find("GET") != string::npos)
        {
            string key = words[1];
            int table = atoi(words[2].c_str());

            if (key.find("*") != string::npos)
            {
                return hashtables[table].GetAll();
            }
            else
            {
                return hashtables[table].Get(key);
            }
        }

        if (words[0].find("DEL") != string::npos)
        {
            string key = words[1];
            int table = atoi(words[3].c_str());
            string mode = words[2];

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
};