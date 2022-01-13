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
    std::vector<Hashtable> hashtables; //set it to public to use it's methods for debug, make this private later...

    Controller(int numTables = 2)
    {
        if (numTables < 2)
        {
            numTables = 2;
        }
        hashtables.resize(numTables); //always 1 more because 0 is for internal use
    }

    int GetSize()
    {
        return hashtables.size();
    }
    std::string tempIP;

    std::string ResolveStringCommand(std::string command)
    {
        std::vector<std::string> words = GetStringValues(command, '/');
        std::vector<std::string> commands = GetStringCommands(command, '/');

        if (VectorContains(commands, "AUTH"))
        {
            if (words.size() == 1)
            {
                std::cout << words[0] << "\n";
                if (hashtables[0].Contains(words[0]))
                {
                    if (!hashtables[0].Contains(tempIP))
                    {
                        hashtables[0].Add(tempIP);
                    }
                }
                return "200 OK";
            }
        }

        else if (hashtables[0].Contains(tempIP))
        {
            if (VectorContains(commands, "INSERT"))
            {
                if (VectorContains(commands, "INTO") && words[1] != "")
                {
                    //int index = std::stoi(words[1]);
                    int index = atoi(words[1].c_str());
                    if (VectorContains(commands, "WHEREKEY") && words.size() == 3)
                    {
                        if (index < hashtables.size())
                        {
                            hashtables[index].Add(words[2], words[0]);
                            return "201 OK";
                        }
                    }
                    else if (!VectorContains(commands, "WHEREKEY"))
                    {
                        if (index < hashtables.size())
                        {
                            hashtables[index].Add(words[0]);
                            return "201 OK";
                        }
                    }
                }
            }

            else if (VectorContains(commands, "DELETE"))
            {
                if (VectorContains(commands, "FROM") && words[1] != "")
                {
                    int index = atoi(words[1].c_str());
                    {
                        if (index < hashtables.size())
                        {
                            hashtables[index].Remove(words[0]);
                            return "204 OK";
                        }
                    }
                }
            }

            else if (VectorContains(commands, "FETCH"))
            {
                if (VectorContains(commands, "FROM") && words[1] != "")
                {
                    int index = atoi(words[1].c_str());
                    {
                        if (words[0].length() == 1 && words[0] == "*")
                        {
                            return hashtables[index].GetAll();
                        }
                        else if (index < hashtables.size())
                        {
                            return hashtables[index].Get(words[0]);
                        }
                    }
                }
            }
            return "404 Command not found.";
        }
        return "401 Unauthorized.";
    }

    std::string GenerateKey(int length = 16)
    {
        srand(time(nullptr));
        std::string key;
        for (int i = 0; i < length; i++)
        {
            switch (rand() % 3)
            {
                case(0): key += std::to_string(rand() % 10); break;
                case(1): key += ('A' + rand() % 26); break;
                case(2): key += ('a' + rand() % 26); break;
            }
        }
        std::string output = key;
        hashtables[0].Add(key);
        return output;
        /*
        File file;

        std::string dbPath = std::string(homedir) + "/LarbyDB/";
        if (!file.DirectoryExists(dbPath))
        {
            file.CreateDirectory(dbPath);
        }

        std::string keyPath = dbPath + "Auth/";
        if (!file.DirectoryExists(keyPath))
        {
            file.CreateDirectory(keyPath);
        }

        std::string keyFile = keyPath + "key.txt";
        if (!file.FileExists(keyFile))
        {
            file.CreateFile(keyFile);
        }
        
        file.OverwriteFile(keyFile, key);
        */
    }

};