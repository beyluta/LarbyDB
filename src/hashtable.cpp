#include "hashtable.h"

using namespace std;
class Hashtable
{
private:
    vector<string> table;
    
public:
    int Hash(string value)
    {
        int hash = 0;
        for (int i = 0; i < value.length(); i++)
        {
            hash += value[i];
        }
        return hash;
    }

    void AddTo(int hash, string &value)
    {
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = move(value);
        }
    }

    void Add(string &value)
    {
        int hash = Hash(value);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = move(value);
        }

        else
        {
            cout << GetHttpStatusCode(507) << endl;
        }
    }

    int Add(string key, string &value)
    {
        int hash = Hash(key);
        if (table.size() <= hash)
        {
            table.resize(hash + 1);
        }

        if (table.at(hash) == "")
        {
            table.at(hash) = move(value);
            return hash;
        }

        else
        {
            cout << GetHttpStatusCode(507) << endl;
        }

        return hash; //TODO: something weird is going on with the returns, check out with the debugger later...
    }

    string Get(string key)
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

    string GetAll()
    {
        string result = "[";
        for (int i = 0; i < table.size(); i++)
        {
            if (table.at(i) != "")
            {
                result += "{\"index\":\"" + to_string(i) + "\",\"value\":\"" + table.at(i) + "\"},";
            }
        }
        if (result[result.size() - 1] == ',')
        {
            result.pop_back();
        }
        return result + "]";
    }

    string GetInRange(int start, int end)
    {
        if (start > end) { return "[]"; }
        string result = "[";
        int a = 0;
        for (int i = 0; i < table.size(); i++)
        {
            if (table.at(i) != "")
            {
                if (a >= start && a <= end)
                {
                    result += "{\"index\":\"" + to_string(i) + "\",\"value\":\"" + table.at(i) + "\"},\n";
                }
                a++;
            }
        }
        if (result[result.size() - 1] == ',')
        {
            result.pop_back();
        }
        return result + "]";
    }

    string GetAmount(int amount, int skip)
    {
        string result = "[";
        int a = 0;
        for (int i = 0; i < table.size(); i++)
        {
            if (table.at(i) != "")
            {
                if (a >= (skip * amount) - amount)
                {
                    result += "{\"index\":\"" + to_string(i) + "\",\"value\":\"" + table.at(i) + "\"},\n";
                }
                a++;
            }
            if (a >= amount * skip) { break; }
        }
        if (result[result.size() - 1] == ',')
        {
            result.pop_back();
        }
        return result + "]";
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

    void Remove(int hash)
    {
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
        if (table.at(hash) != "" && table.at(hash) == value)
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
                cout << "  [" << i << "]: " << table.at(i) << "\n";
            }
        }
    }
};