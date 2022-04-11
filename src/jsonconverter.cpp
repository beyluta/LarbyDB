#include "jsonconverter.h"

class JsonParser
{
public:
    enum JsonType
    {
        JSON_OBJECT = 0,
        JSON_ARRAY = 1,
        JSON_STRING = 2,
        JSON_NUMBER = 3,
        JSON_TRUE = 4,
        JSON_FALSE = 5,
        JSON_NULL = 6
    };

    string Get(string json)
    {
        return json;
    }

    JsonType GetType(string json)
    {
        json.erase(std::remove_if(json.begin(), json.end(), ::isspace), json.end());
        int end = json.length() - 1;

        if (json[0] == '{' && json[end] == '}')
        {
            return JSON_OBJECT;
        }
        if (json[0] == '[' && json[end] == ']')
        {
            return JSON_ARRAY;
        }

        return JSON_NULL;
    }
};

class Json
{
private:
    vector<string> values;

    int Hash(string key)
    {
        int hash = 0;
        for (int i = 0; i < key.size(); i++)
        {
            hash += key[i];
        }
        return hash;
    }

    bool Contains(string key)
    {
        if (values.size() < Hash(key))
        {
            return false;
        }

        return values[Hash(key)].length() > 0 ? true : false;
    }

    void Add(string key, string value)
    {
        if (values.size() < Hash(key))
        {
            values.resize(Hash(key) + 1);
        }

        if (!Contains(key))
        {
            values[Hash(key)] = value;
        }
    }

    string Get(string key)
    {
        return Contains(key) ? values[Hash(key)] : "";
    }

    vector<int> GetCharCount(string includedCharacters, string json)
    {
        vector<int> charIndexes;

        for (int i = 0; i < json.length(); i++)
        {
            for (int j = 0; j < includedCharacters.length(); j++)
            {
                if (json[i] == includedCharacters[j])
                {
                    charIndexes.push_back(i);
                }
            }
        }

        return charIndexes;
    }

    string GetSubstringBetweenIndices(string str, int start, int end)
    {
        string result;

        for (int i = start + 1; i < end; i++)
        {
            result += str[i];
        }

        return result;
    }

public:
    Json(string json)
    {
        vector<int> charIndexes = GetCharCount("\"[]{}:,", json);
        string lastValue;
        int count = 1;

        for (int i = 0; i < charIndexes.size(); i++)
        {
            char charPtr = json[charIndexes[i]];
            char nextCharPtr = json[charIndexes[i + 1]];
            
            if (charPtr == nextCharPtr)
            {
                int currentIndex = charIndexes[i];
                int nextIndex = charIndexes[i + 1];
                string substr = GetSubstringBetweenIndices(json, currentIndex, nextIndex);
                
                if (count <= 0)
                {
                    Add(lastValue, substr);
                    count = 2;
                }

                count--;
                lastValue = substr;
                i++;
            }
        }
    }

    string operator[](string key)
    {
        return Get(key);
    }
};

int main()
{
    // JsonParser parser;
    // cout << parser.GetType("[{ \"key\" : \"value\"}]") << "\n";
    Json json("{ \"key\" : \"value\", \"name\" : \"pedro\" }");
    return 0;
}