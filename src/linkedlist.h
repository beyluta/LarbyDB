#pragma once
#include <iostream>

namespace LinkedList
{
    class SLList
    {
        struct Node
        {
            std::string* data;
            Node* next;
        };

        Node* first;
        Node* last;

        int length;
        void OnDestruct();

    public:

        SLList();
        ~SLList();

        Node* First();
        Node* Last();

        int GetLength();

        void AddToEnd(std::string d);
        void Insert(std::string d, unsigned int pos);
        void Push(std::string d);
        std::string Pop();
        std::string Peek();

        void PrintList();
        std::string ToString();
        std::string* ToArray();
    };
}