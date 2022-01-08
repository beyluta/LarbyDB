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

    Controller(int numTables = 1)
    {
        if (numTables < 1)
        {
            numTables = 1;
        }
        hashtables.resize(numTables);
    }

    void ResolveStringCommand(std::string command)
    {
        std::vector<std::string> words = GetStringValues(command, '/');
        std::vector<std::string> commands = GetStringCommands(command, '/');

        if (VectorContains(commands, "INSERT"))
        {
            if (VectorContains(commands, "INTO") && words[1] != "")
            {
                //int index = std::stoi(words[1]);
                int index = atoi(words[1].c_str());
                {
                    if (index < hashtables.size())
                    {
                        hashtables[index].Add(words[0]);
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
                        {
                            hashtables[index].Remove(words[0]);
                        }
                    }
                }
            }
        }


    }
};