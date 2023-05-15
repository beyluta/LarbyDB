#pragma once
#include <iostream>
#include <vector>
#include "linknode.h"

using namespace std;

class Hashtable
{
private:
    vector<LinkNode> table;

public:
    int Hash(string value);
    void AddTo(int hash, string value);
    void Add(string value);
    int Add(string key, string value);
    string Get(string key);
    string GetAll();
    string GetInRange(int start, int end);
    string GetAmount(int amount, int skip);
    void Remove(string value);
    void Remove(int hash);
    bool Contains(string value);
};
