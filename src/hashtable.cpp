#include "hashtable.h"

using namespace std;
using namespace HashtableUtility;

vector<string> table;

int Hashtable::Hash(string value)
{
    int hash = 0;

    for (int i = 0; i < value.length(); i++)
    {
        hash += value[i];
    }

    return hash;
}

void Hashtable::AddTo(int hash, string value)
{
    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    if (table.at(hash) == "")
    {
        table.at(hash) = value;
    }
}

void Hashtable::Add(string value)
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
}

int Hashtable::Add(string key, string value)
{
    int hash = Hash(key);

    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    if (table.at(hash) == "")
    {
        table.at(hash) = value;
        return hash;
    }

    return hash; // TODO: something weird is going on with the returns, check out with the debugger later...
}

string Hashtable::Get(string key)
{
    int hash = Hash(key);

    if (table.size() <= hash)
    {
        return "";
    }

    return table.at(hash) == "" ? "" : table.at(hash);
}

string Hashtable::GetAll()
{
    string result = "[";

    for (int i = 0; i < table.size(); i++)
    {
        if (table.at(i) != "")
        {
            string value = table.at(i)[0] == '{' || table.at(i)[0] == '[' ? "\"value\":" + table.at(i) + "}," : "\"value\":\"" + table.at(i) + "\"},";
            result += "{\"index\":\"" + to_string(i) + "\"," + value;
        }
    }

    if (result[result.size() - 1] == ',')
    {
        result.pop_back();
    }

    return result + "]";
}

string Hashtable::GetInRange(int start, int end)
{
    if (start > end)
    {
        return "[]";
    }
    string result = "[";
    int a = 0;

    for (int i = 0; i < table.size(); i++)
    {
        if (table.at(i) != "")
        {
            if (a >= start && a <= end)
            {
                string value = table.at(i)[0] == '{' || table.at(i)[0] == '[' ? "\"value\":" + table.at(i) + "}," : "\"value\":\"" + table.at(i) + "\"},\n";
                result += "{\"index\":\"" + to_string(i) + "\"," + value;
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

string Hashtable::GetAmount(int amount, int skip)
{
    string result = "[";
    int a = 0;

    for (int i = 0; i < table.size(); i++)
    {
        if (table.at(i) != "")
        {
            if (a >= (skip * amount) - amount)
            {
                string value = table.at(i)[0] == '{' || table.at(i)[0] == '[' ? "\"value\":" + table.at(i) + "}," : "\"value\":\"" + table.at(i) + "\"},\n";
                result += "{\"index\":\"" + to_string(i) + "\"," + value;
            }

            a++;
        }

        if (a >= amount * skip)
        {
            break;
        }
    }

    if (result[result.size() - 1] == ',')
    {
        result.pop_back();
    }

    return result + "]";
}

void Hashtable::Remove(string value)
{
    int hash = Hash(value);

    if (table.size() <= hash)
    {
        return;
    }

    table.at(hash) = "";
}

void Hashtable::Remove(int hash)
{
    if (table.size() <= hash)
    {
        return;
    }

    table.at(hash) = "";
}

bool Hashtable::Contains(string value)
{
    int hash = Hash(value);

    if (table.size() <= hash)
    {
        return false;
    }

    return table.at(hash) != "" && table.at(hash) == value ? true : false;
}
