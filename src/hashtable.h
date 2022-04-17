#pragma once
#include <iostream>
#include <vector>

using namespace std;

namespace HashtableUtility
{
    class Hashtable
    {
    private:
        vector<string> table;

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
}