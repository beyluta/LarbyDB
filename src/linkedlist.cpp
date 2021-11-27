#include "linkedlist.h"
#include <string>
#include <iostream>

namespace LinkedList
{
    SList::SList()
    {
        first = nullptr;
        last = nullptr;
    }

    void SList::AddToEnd(std::string data)
    {
        Node* temp = new Node;
        temp->data = new std::string(data);
        temp->next = nullptr;
        
        if (first == nullptr)
        {
            first = temp;
            last = temp;
            return;
        }
        last->next = temp;
        last = temp;
    }

    void SList::Push(std::string data)
    {
        Node* temp = new Node;
        temp->data = new std::string(data);
        temp->next = first;

        if (first == nullptr)
        {
            last = temp;
        }
        first = temp;
    }

    std::string* SList::Pop()
    {
        try
        {
            if (first == nullptr)
            {
                throw "list empty, nothing to remove.";
            }
            Node* temp = first;
            std::string* deletedVal = first->data;
            
            delete first->data;
            delete first;
            first = temp;
            return deletedVal;
        }
        catch (const char* msg)
        {
            std::cerr << msg << '\n';
            return nullptr;
        }
    }

    void SList::OnDestruct()
    {
        if (first != nullptr)
        {
            this->Pop();
            this->OnDestruct();
        }
        else
            std::cout << "list deleted";
    }

    SList::~SList()
    {
        this->OnDestruct();
    }

#ifdef DEBUG
    void SList::PrintLIst()
    {
        std::cout << "Linked list:\n";
        if (first == nullptr)
        {
            std::cout << "(nothing)\n";
            return;
        }
        Node* temp = first;
        std::cout << "[ ";
        while (temp->next != nullptr)
        {
            std::cout << *temp->data << " ; ";
            temp = temp->next;
        }
        std::cout << *temp->data << " ]\n";
    }
#endif
}