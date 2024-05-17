#include "hashtable.h"

Hashtable::Hashtable()
{
    Hashtable::table.reserve(MAX_CAPACITY);
}

Hashtable::Hashtable(std::vector<std::string> values) : Hashtable()
{
    for (int i = 0; i < values.size(); i++)
    {
        Hashtable::Add(values.at(i));
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

int Hashtable::Hash()
{
    if (values.size() <= 0)
    {
        return 0;
    }

    Sort::MergeSort(values);

    int index = values[0] - 1;

    if (index < 0)
    {
        index = values[values.size() - 1] + 1;

        if (index >= MAX_CAPACITY)
        {
            return -1;
        }
    }

    return index;
}

int Hashtable::Hash(std::string value)
{
    unsigned long hash = 5381;

    for (int i = 0; i < value.size(); i++)
    {
        hash = ((hash << 5) + hash) + value[i];
    }

    return hash % MAX_CAPACITY;
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

int Hashtable::Add(std::string value, bool autoIncrement, int &newHash)
{
    int hash = autoIncrement ? Hash() : Hash(value);

    if (autoIncrement)
    {
        newHash = hash;
    }

    if (hash < 0)
    {
        return 1;
    }

    if (table.size() <= hash)
    {
        table.resize(hash + 1);
    }

    LinkedList::Add(&table.at(hash), value);
    Hashtable::UpdateIndex(hash, Hashtable::IndexUpdateOperation::Add);
    return 0;
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

std::string Hashtable::Get(int key)
{
    if (table.size() <= key)
    {
        return "";
    }

    std::string values = GetSameIndexValues(&table.at(key));

    if (values != "")
    {
        return values;
    }

    return table.at(key).value;
}

std::string Hashtable::GetAll()
{
    std::string result = "[";

    for (int i = 0; i < Hashtable::values.size(); i++)
    {
        std::string values = GetSameIndexValues(&table.at(Hashtable::values.at(i)));
        std::string value = table.at(Hashtable::values.at(i)).value[0] == '{' || table.at(Hashtable::values.at(i)).value[0] == '[' ? "\"value\":" + table.at(Hashtable::values.at(i)).value + "}," : "\"value\":\"" + table.at(Hashtable::values.at(i)).value + "\"},";
        result += "{\"index\":\"" + std::to_string(Hashtable::values.at(i)) + "\"," + "\"total\":\"" + std::to_string(Hashtable::Length()) + "\"," + (values != "" ? "\"value\":" + values + "}," : value);
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
            result += "{\"index\":\"" + std::to_string(Hashtable::values.at(i)) + "\"," + "\"total\":\"" + std::to_string(Hashtable::Length()) + "\"," + (values != "" ? "\"value\":" + values + "}," : value);
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
            result += "{\"index\":\"" + std::to_string(Hashtable::values.at(i)) + "\"," + "\"total\":\"" + std::to_string(Hashtable::Length()) + "\"," + (values != "" ? "\"value\":" + values + "}," : value);
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
