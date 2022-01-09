#include "hashtable.h"

using namespace std;
class Hashtable
{
private:
    vector<string> table;

    int Hash(string value)
    {
        int hash = 0;
        for (int i = 0; i < value.length(); i++)
        {
            hash += value[i];
        }
        return hash;
    }

public:
    void Add(string &value)
    {
        int hash = Hash(value);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = std::move(value);
        }

        else
        {
            std::cout << "Internal Server Error (507): Hash Collision Detected\n";
        }
    }

    void Add(string key, string &value)
    {
        int hash = Hash(key);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = std::move(value);
        }

        else
        {
            std::cout << "Internal Server Error (507): Hash Collision Detected\n";
        }
    }

    std::string Get(string key)
    {
        int hash = Hash(key);
        if (table.size() <= hash)
        {
            return "";
        }

        if (table.at(hash) == "")
        {
            return "";
        }

        else
        {
            return table.at(hash);
        }
    }

    std::string GetAll()
    {
        std::string result = "";
        for (int i = 0; i < table.size(); i++)
        {
            if (table.at(i) != "")
            {
                result += to_string(i) + ":" + table.at(i) + ";";
            }
        }
        return result;
    }

    void Remove(string value)
    {
        int hash = Hash(value);
        if (table.size() <= hash)
        {
            return;
        }

        table.at(hash) = "";
    }

    bool Contains(string value)
    {
        int hash = Hash(value);
        if (table.size() <= hash)
        {
            return false;
        }
        if (table.at(hash) == value)
        {
            return true;
        }
        return false;
    }

    void PrintTable(int id = 0)
    {
        cout << "Hashtable[" << id << "]:\n{\n";
        for (int i = 0; i < table.size(); i++)
        {
            if (table.at(i) != "")
            {
                cout << "  [" << i << "]: " << table.at(i) << "\n";
            }
        }
        cout << "}" << endl;
    }
};