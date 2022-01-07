#include "hashtable.h"

class Hashtable
{
private:
    vector<string> table;

public:
    int Hash(string key)
    {
        int hash = 0;
        for (int i = 0; i < key.length(); i++)
        {
            hash += key[i];
        }
        return hash;
    }

    void Add(string key)
    {
        int hash = Hash(key);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = key;
        }
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