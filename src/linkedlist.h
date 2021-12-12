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

        Node* First() const;
        Node* Last() const;
        int GetLength() const;

        void AddToEnd(const char* d);
        void Insert(const char* d, unsigned int pos);
        void Delete(const char* string);
        void Push(const char* d);
        const char* Pop();
        const char* Peek();

        void PrintList();
        const char** ToArray();
        bool Contains(const char* input);
    };
}