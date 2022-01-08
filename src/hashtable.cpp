#include "hashtable.h"

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
    void Add(string value)
    {
        int hash = Hash(value);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = value;
        }
        
        else
        {
            std::cout << "Internal Server Error (507): Hash Collision Detected\n";
        }
    }

    void Add(string key, string value)
    {
        int hash = Hash(key);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = value;
        }
        
        else
        {
            std::cout << "Internal Server Error (507): Hash Collision Detected\n";
        }
    }

    void Remove(string value)
    {
        int hash = Hash(value);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == value)
        {
            table.at(hash) = "";
        }
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

    void PrintTable()
    {
        for (int i = 0; i < table.size(); i++)
        {
            if (table.at(i) != "")
            {
                cout << table.at(i) << endl;
            }
        }
    }
};