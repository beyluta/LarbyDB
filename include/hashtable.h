#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "linknode.h"
#include "sort.h"

#define MAX_CAPACITY 2'000'000

class Hashtable
{
private:
    enum class IndexUpdateOperation
    {
        Add,
        Remove
    };
    std::vector<LinkNode> table;
    std::vector<int> indexes;
    std::vector<int> values;
    void UpdateIndex(int index, IndexUpdateOperation operation);


public:
    Hashtable();
    Hashtable(std::vector<std::string> values);
    std::string GetSameIndexValues(LinkNode *node);
    int Hash();
    int Hash(std::string value);
    void AddTo(int hash, std::string value);
    int Add(std::string value, bool autoIncrement = false);
    int Add(std::string key, std::string value);
    std::string Get(std::string key);
    std::string GetAll();
    std::string GetInRange(int start, int end);
    std::string GetAmount(int amount, int skip);
    void Remove(std::string value);
    void Remove(int hash);
    bool Contains(std::string value);
    int Length();
};
