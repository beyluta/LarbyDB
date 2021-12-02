#pragma once

namespace LinkedList
{
    class SLList
    {
        struct Node
        {
            const char** data;
            Node* next;
        };

        Node* first;
        Node* last;

        int length;

    public:

        SLList();
        ~SLList();

        Node* First();
        Node* Last();

        int GetLength();

        void AddToEnd(const char* d);
        void Insert(const char* d, unsigned int pos);
        void Push(const char* d);
        const char* Pop();
        const char* Peek();

        void PrintList();
        const char** ToArray();
        const char** ToCharArray();
        bool Contains(std::string input);
    };
}