#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "linknode.h"

class Hashtable
{
private:
    std::vector<LinkNode> table;

public:
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
};
