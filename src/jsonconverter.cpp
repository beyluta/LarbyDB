#include "jsonconverter.h"

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
        int openedBracesOrBrackets = 0;

        for (int i = 0; i < json.length(); i++)
        {
            for (int j = 0; j < includedCharacters.length(); j++)
            {
                if (json[i] == includedCharacters[j] && json[i] == '[' || json[i] == '{')
                {
                    if (openedBracesOrBrackets <= 0)
                    {
                        charIndexes.push_back(i);
                    }

                    openedBracesOrBrackets++;
                }
                else if (json[i] == includedCharacters[j] && json[i] == ']' || json[i] == '}')
                {
                    if (openedBracesOrBrackets <= 1)
                    {
                        charIndexes.push_back(i);
                    }

                    openedBracesOrBrackets--;
                }
                else if (json[i] == includedCharacters[j] && openedBracesOrBrackets <= 0)
                {
                    charIndexes.push_back(i);
                }
            }
        }     

        return charIndexes;
    }

    string GetSubstringBetweenIndices(string str, int start, int end, bool exclusive = true)
    {
        string result;
        end = !exclusive ? end += 1 : end;

        for (int i = start + exclusive; i < end; i++)
        {
            result += str[i];
        }

        return result;
    }

    bool CompareAssert(char a, char b)
    {    
        if (a == b)
        {
            return true;
        }
        else if (a == '[' && b == ']' || a == '{' && b == '}')
        {
            return true;
        }

        return false;
    }

    string TrimString(string str) 
    {
        const char* typeOfWhitespaces = " \t\n\r\f\v";
        str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
        str.erase(0,str.find_first_not_of(typeOfWhitespaces));
        return str;
    }

public:
    string json;

    Json(string json)
    {
        json = TrimString(json);
        this->json = json;
        int removeCharsCount = 0;

        for (int i = 0; i < json.length(); i++)
        {
            if (json[i] == '\"')
            {
                json.erase(0, removeCharsCount);
                json.erase(json.length() - removeCharsCount, removeCharsCount);
                break;
            }

            removeCharsCount++;
        }
        
        vector<int> charIndexes = GetCharCount("\"[]{}", json);
        string lastValue;
        int count = 1;

        for (int i = 0; i < charIndexes.size(); i++)
        {            
            if (CompareAssert(json[charIndexes[i]], json[charIndexes[i + 1]]))
            {
                bool isEnclosedInBracketsOrBraces = CompareAssert(json[charIndexes[i]], '[') || CompareAssert(json[charIndexes[i]], '{') ? true : false;
                string substr = count > 0 ? GetSubstringBetweenIndices(json, charIndexes[i], charIndexes[i + 1]) :
                GetSubstringBetweenIndices(
                    json,
                    charIndexes[i],
                    charIndexes[i + 1],
                    !isEnclosedInBracketsOrBraces
                );
                
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

    string operator[] (int key)
    {
        return "";
    }

    string operator[](string key)
    {
        return Get(key);
    }
};

int main()
{
    Json json("{ \"people\" : [{ \"name\":\"beyluta\" }], \"single\":\"pedro :<\" }");
    cout << json["people"] << endl;
    Json json2(json["people"]);
    cout << json2["name"] << endl;
    return 0;
}