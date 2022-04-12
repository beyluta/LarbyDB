#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

namespace JsonUtilities
{
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
        string Get(string json);
        string GetType(string json);
    };

    class Json
    {
    private:
        vector<string> values;
        int Hash(string key);
        bool Contains(string key);
        void Add(string key, string value);
        string Get(string key);
        vector<int> GetCharCount(string includedCharacters, string json);
        string GetSubstringBetweenIndices(string str, int start, int end);
        bool CompareAssert(char a, char b);

    public:
        Json(string json);
        string operator[](string key);
    };
}