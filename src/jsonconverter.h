#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

namespace JsonUtilities
{
    class Json
    {
    private:
        vector<string> values;
        int Hash(string key);
        bool Contains(string key);
        void Add(string key, string value);
        string Get(string key);
        vector<int> GetCharCount(string includedCharacters, string json);
        string GetSubstringBetweenIndices(string str, int start, int end, bool exclusive = true);
        bool CompareAssert(char a, char b);
        string TrimString(string str);
        string GetObjectFromJsonArray(int index);

    public:
        string json;
        Json(string json);
        string operator[](int key);
        string operator[](string key);
    };
}