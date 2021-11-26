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

            ~Node()
            {
                //data->~basic_string();
                delete data;
                delete next;
            }
        };

        Node* first;
        Node* last;

        void OnDestruct();

    public:

        SLList();
        ~SLList();

        Node* First();
        Node* Last();

        void AddToEnd(std::string& d);
        void Insert(std::string& d, unsigned int pos);
        void Push(std::string& d);
        std::string Pop();

        void PrintList();
    };
}