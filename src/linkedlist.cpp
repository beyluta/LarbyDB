#include "linkedlist.h"
#include <string>
#include <iostream>

namespace LinkedList
{
    SLList::SLList()
    {
        first = nullptr;
        last = nullptr;
    }
    
    void SLList::OnDestruct()
    {
        if (first != nullptr)
        {
            this->Pop();
            this->OnDestruct();
            return;
        }
        std::cout << "linked list deleted\n";
    }

    SLList::~SLList()
    {
        this->OnDestruct();
    }

    SLList::Node* SLList::First()
    {
        return first;
    }

    SLList::Node* SLList::Last()
    {
        return last;
    }

    void SLList::AddToEnd(std::string d)
    {
        Node* temp = new Node;
        temp->data = &d;
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

    void SLList::Insert(std::string d, unsigned int pos)
    {
        std::cout << "[inserting(data: " << d << ", pos: " << pos << ")]: ";
        if (first == nullptr)
        {
            std::cout << "list was empty, adding as first element\n";
            Push(d);
            return;
        }

        int currentPos = 0;
        Node** ptr = &first;

        while (currentPos != pos && (*ptr)->next != nullptr)
        {
            ptr = &(*ptr)->next;
            currentPos++;
        }

        Node* newNode = new Node;
        newNode->data = &d;
        newNode->next = nullptr;

        if ((*ptr)->next == nullptr)
        {
            std::cout << "end of the list reached, adding as last element\n"; //debug
            (*ptr)->next = newNode;
            last = newNode;
            return;
        }
        std::cout << "after " << *(*ptr)->data << " before " << *(*ptr)->next->data << '\n'; //debug

        newNode->next = (*ptr)->next;
        (*ptr)->next = newNode;
    }

    void SLList::Push(std::string d)
    {
        Node* temp = new Node;
        temp->data = &d;
        temp->next = first;

        if (first == nullptr)
        {
            last = temp;
        }

        first = temp;
    }

    std::string SLList::Peek()
    {
        if (first == nullptr)
        {
            return "";
        }
        return *(first->data);
    }

    std::string SLList::Pop()
    {
        try
        {
            if (first == nullptr)
            {
                throw "Cannot remove elements from an empty list!";
            }
            Node* temp = first;
            std::string deletedValue = *first->data;
            temp = first->next;
            first->data = nullptr;
            delete first;
            first = temp;
            return deletedValue;
        }
        catch (const char* msg)
        {
            std::cerr << msg << std::endl;
            return nullptr;
        }
    }

#ifdef DEBUG
    void SLList::PrintList()
    {
        std::cout << "Linked list: ";
        if (first == nullptr)
        {
            std::cout << "[ empty list ]\n\n";
            return;
        }
        Node* temp = first;
        std::cout << "[ ";
        while (temp->next != nullptr)
        {
            std::cout << *temp->data << " > ";
            temp = temp->next;
        }
        std::cout << *temp->data << " ]\n";
    }
#endif
}