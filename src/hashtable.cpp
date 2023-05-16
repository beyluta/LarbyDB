#include "hashtable.h"

std::string Hashtable::GetSameIndexValues(LinkNode *node)
{
    std::vector<std::string> list = LinkedList::List(node);
    std::string values = "[";
    if (list.size() > 1) {
        for (int j = 0; j < list.size(); j++)
        {
            values += list.at(j) + ",";
        }
    } else {
        return "";
    }
    values.pop_back();
    values += "]";
    return values;
}

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

    LinkedList::Add(&table.at(hash), value);
}

void Hashtable::Add(string value)
{
    int hash = Hash(value);

    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    LinkedList::Add(&table.at(hash), value);
}

int Hashtable::Add(string key, string value)
{
    int hash = Hash(key);

    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    LinkedList::Add(&table.at(hash), value);

    return hash;
}

string Hashtable::Get(string key)
{
    int hash = Hash(key);

    if (table.size() <= hash)
    {
        return "";
    }

    std::string values = GetSameIndexValues(&table.at(hash));

    if (values != "") {
        return values;
    }

    return table.at(hash).value;
}

string Hashtable::GetAll()
{
    string result = "[";

    for (int i = 0; i < table.size(); i++)
    {
        if (table.at(i).value != "")
        {
            std::string values = GetSameIndexValues(&table.at(i));
            string value = table.at(i).value[0] == '{' || table.at(i).value[0] == '[' ? "\"value\":" + table.at(i).value + "}," : "\"value\":\"" + table.at(i).value + "\"},";
            result += "{\"index\":\"" + to_string(i) + "\"," + (values != "" ? "\"value\":" + values + "}," : value);
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
        if (table.at(i).value != "")
        {
            if (a >= start && a <= end)
            {
                std::string values = GetSameIndexValues(&table.at(i));
                string value = table.at(i).value[0] == '{' || table.at(i).value[0] == '[' ? "\"value\":" + table.at(i).value + "}," : "\"value\":\"" + table.at(i).value + "\"},\n";
                result += "{\"index\":\"" + to_string(i) + "\"," + (values != "" ? "\"value\":" + values + "}," : value);
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
        if (table.at(i).value != "")
        {
            if (a >= (skip * amount) - amount)
            {
                std::string values = GetSameIndexValues(&table.at(i));
                string value = table.at(i).value[0] == '{' || table.at(i).value[0] == '[' ? "\"value\":" + table.at(i).value + "}," : "\"value\":\"" + table.at(i).value + "\"},\n";
                result += "{\"index\":\"" + to_string(i) + "\"," + (values != "" ? "\"value\":" + values + "}," : value);
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

    LinkedList::Unlink(&table.at(hash));
    table.at(hash) = LinkNode();
}

void Hashtable::Remove(int hash)
{
    if (table.size() <= hash)
    {
        return;
    }

    LinkedList::Unlink(&table.at(hash));
    table.at(hash) = LinkNode();
}

bool Hashtable::Contains(string value)
{
    int hash = Hash(value);

    if (table.size() <= hash)
    {
        return false;
    }

    return table.at(hash).value != "" && table.at(hash).value == value ? true : false;
}
