#pragma once
#include <string>
namespace LinkedList
{
    struct Node
    {
        std::string* data;
        Node* next;
    };

    class SList
    {
        Node* first;
        Node* last;

        void OnDestruct();

        public:

        Node* First()
        {
            return first;
        }

        Node* Last()
        {
            return last;
        }

        SList();
        void AddToEnd(std::string data);
        void Push(std::string data);
        std::string* Pop();
        ~SList();
#ifdef DEBUG
        void PrintLIst();
#endif
    };
}