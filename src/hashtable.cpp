#include "hashtable.h"

Hashtable::Hashtable() {}

Hashtable::Hashtable(std::vector<std::string> values)
{
    for (int i = 0; i < values.size(); i++)
    {
        Add(values.at(i));
    }
}

std::string Hashtable::GetSameIndexValues(LinkNode *node)
{
    std::vector<std::string> list = LinkedList::List(node);
    std::string values = "[";
    if (list.size() > 1)
    {
        for (int j = 0; j < list.size(); j++)
        {
            values += list.at(j) + ",";
        }
    }
    else
    {
        return "";
    }
    values.pop_back();
    values += "]";
    return values;
}

int Hashtable::Hash(std::string value)
{
    int hash = 0;

    for (int i = 0; i < value.length(); i++)
    {
        hash += value[i];
    }

    return hash;
}

void Hashtable::AddTo(int hash, std::string value)
{
    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    LinkedList::Add(&table.at(hash), value);
    Hashtable::UpdateIndex(hash, Hashtable::IndexUpdateOperation::Add);
}

void Hashtable::Add(std::string value)
{
    int hash = Hash(value);

    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    LinkedList::Add(&table.at(hash), value);
    Hashtable::UpdateIndex(hash, Hashtable::IndexUpdateOperation::Add);
}

int Hashtable::Add(std::string key, std::string value)
{
    int hash = Hash(key);

    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    LinkedList::Add(&table.at(hash), value);
    Hashtable::UpdateIndex(hash, Hashtable::IndexUpdateOperation::Add);
    return hash;
}

std::string Hashtable::Get(std::string key)
{
    int hash = Hash(key);

    if (table.size() <= hash)
    {
        return "";
    }

    std::string values = GetSameIndexValues(&table.at(hash));

    if (values != "")
    {
        return values;
    }

    return table.at(hash).value;
}

std::string Hashtable::GetAll()
{
    std::string result = "[";

    for (int i = 0; i < Hashtable::values.size(); i++)
    {
        std::string values = GetSameIndexValues(&table.at(Hashtable::values.at(i)));
        std::string value = table.at(Hashtable::values.at(i)).value[0] == '{' || table.at(Hashtable::values.at(i)).value[0] == '[' ? "\"value\":" + table.at(Hashtable::values.at(i)).value + "}," : "\"value\":\"" + table.at(Hashtable::values.at(i)).value + "\"},";
        result += "{\"index\":\"" + std::to_string(Hashtable::values.at(i)) + "\"," + "\"total\":" + std::to_string(Hashtable::Length()) + "," + (values != "" ? "\"value\":" + values + "}," : value);
    }

    if (result[result.size() - 1] == ',')
    {
        result.pop_back();
    }

    return result + "]";
}

std::string Hashtable::GetInRange(int start, int end)
{
    if (start > end)
    {
        return "[]";
    }
    std::string result = "[";
    int a = 0;

    for (int i = 0; i < Hashtable::values.size(); i++)
    {
        if (a >= start && a <= end)
        {
            std::string values = GetSameIndexValues(&table.at(Hashtable::values.at(i)));
            std::string value = table.at(Hashtable::values.at(i)).value[0] == '{' || table.at(Hashtable::values.at(i)).value[0] == '[' ? "\"value\":" + table.at(Hashtable::values.at(i)).value + "}," : "\"value\":\"" + table.at(Hashtable::values.at(i)).value + "\"},\n";
            result += "{\"index\":\"" + std::to_string(Hashtable::values.at(i)) + "\"," + "\"total\":" + std::to_string(Hashtable::Length()) + "," + (values != "" ? "\"value\":" + values + "}," : value);
        }

        a++;
    }

    if (result[result.size() - 1] == ',')
    {
        result.pop_back();
    }

    return result + "]";
}

std::string Hashtable::GetAmount(int amount, int skip)
{
    std::string result = "[";
    int a = 0;

    for (int i = 0; i < Hashtable::values.size(); i++)
    {

        if (a >= (skip * amount) - amount)
        {
            std::string values = GetSameIndexValues(&table.at(Hashtable::values.at(i)));
            std::string value = table.at(Hashtable::values.at(i)).value[0] == '{' || table.at(Hashtable::values.at(i)).value[0] == '[' ? "\"value\":" + table.at(Hashtable::values.at(i)).value + "}," : "\"value\":\"" + table.at(Hashtable::values.at(i)).value + "\"},\n";
            result += "{\"index\":\"" + std::to_string(Hashtable::values.at(i)) + "\"," + "\"total\":" + std::to_string(Hashtable::Length()) + "," + (values != "" ? "\"value\":" + values + "}," : value);
        }

        a++;

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

void Hashtable::Remove(std::string value)
{
    int hash = Hash(value);

    if (table.size() <= hash)
    {
        return;
    }

    LinkedList::Unlink(&table.at(hash));
    Hashtable::UpdateIndex(hash, Hashtable::IndexUpdateOperation::Remove);
    table.at(hash) = LinkNode();
}

void Hashtable::Remove(int hash)
{
    if (table.size() <= hash)
    {
        return;
    }

    LinkedList::Unlink(&table.at(hash));
    Hashtable::UpdateIndex(hash, Hashtable::IndexUpdateOperation::Remove);
    table.at(hash) = LinkNode();
}

bool Hashtable::Contains(std::string value)
{
    int hash = Hash(value);

    if (table.size() <= hash)
    {
        return false;
    }

    return table.at(hash).value != "" && table.at(hash).value == value ? true : false;
}

void Hashtable::UpdateIndex(int index, Hashtable::IndexUpdateOperation operation)
{
    if (operation == Hashtable::IndexUpdateOperation::Add)
    {
        if (Hashtable::indexes.size() <= index)
        {
            Hashtable::indexes.resize(index + 1);
        }

        if (Hashtable::indexes.at(index) != 0)
        {
            return;
        }

        Hashtable::indexes.at(index) = index;
        Hashtable::values.push_back(index);
    }
    else if (operation == Hashtable::IndexUpdateOperation::Remove)
    {
        if (Hashtable::indexes.size() > index)
        {
            if (LinkedList::Length(&table.at(index)) > 2)
            {
                return;
            }

            Hashtable::indexes.at(index) = 0;

            for (int i = 0; i < Hashtable::values.size(); i++)
            {
                if (Hashtable::values.at(i) == index)
                {
                    Hashtable::values.erase(Hashtable::values.begin() + i);
                    return;
                }
            }
        }
    }
}

int Hashtable::Length()
{
    return Hashtable::values.size();
}
