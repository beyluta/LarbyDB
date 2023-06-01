#pragma once
#include <iostream>
#include <vector>

class Json
{
private:
    std::vector<std::string> values;
    int Hash(std::string key);
    bool Contains(std::string key);
    void Add(std::string key, std::string value);
    std::string Get(std::string key);
    std::vector<int> GetCharCount(std::string includedCharacters, std::string json);
    std::string GetSubstringBetweenIndices(std::string str, int start, int end, bool exclusive = true);
    bool CompareAssert(char a, char b);
    std::string TrimString(std::string str);
    std::string GetObjectFromJsonArray(int index);

public:
    std::string json;
    Json(std::string json);
    std::string operator[](int key);
    std::string operator[](std::string key);
};
