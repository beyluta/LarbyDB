#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "linknode.h"

#define MAX_CAPACITY 2'000'000

class Hashtable
{
private:
    std::vector<LinkNode> table;
    std::vector<int> indexes;
    std::vector<int> values;
    enum class IndexUpdateOperation
    {
        Add,
        Remove
    };

public:
    Hashtable();
    Hashtable(std::vector<std::string> values);
    std::string GetSameIndexValues(LinkNode *node);
    int Hash(std::string value);
    void AddTo(int hash, std::string value);
    void Add(std::string value);
    int Add(std::string key, std::string value);
    std::string Get(std::string key);
    std::string GetAll();
    std::string GetInRange(int start, int end);
    std::string GetAmount(int amount, int skip);
    void Remove(std::string value);
    void Remove(int hash);
    bool Contains(std::string value);
    void UpdateIndex(int index, IndexUpdateOperation operation);
    int Length();
};
