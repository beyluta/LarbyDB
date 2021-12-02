#include "linkedlist.h"
#include <iostream>

namespace LinkedList
{
    SLList::SLList()
    {
        first = nullptr;
        last = nullptr;
        length = 0;
    }
    
    SLList::~SLList()
    {
        while (first != nullptr)
        {
            this->Pop();
        }
    }

    SLList::Node* SLList::First()
    {
        return first;
    }

    SLList::Node* SLList::Last()
    {
        return last;
    }

    int SLList::GetLength()
    {
        return length;
    }

    void SLList::AddToEnd(const char* d)
    {
        length++;
        Node* temp = new Node;
        const char** str = new const char*(d);
        temp->data = str;
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

    void SLList::Insert(const char* d, unsigned int pos)
    {
        //std::cout << "[inserting(data: " << d << ", pos: " << pos << ")]: ";
        if (first == nullptr)
        {
            //std::cout << "list was empty, adding as first element\n";
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

        length++;
        Node* newNode = new Node;
        const char** str = new const char*(d);
        newNode->data = str;
        newNode->next = nullptr;

        if ((*ptr)->next == nullptr)
        {
            //std::cout << "end of the list reached, adding as last element\n"; //debug
            (*ptr)->next = newNode;
            last = newNode;
            return;
        }
        //std::cout << "after " << *(*ptr)->data << " before " << *(*ptr)->next->data << '\n'; //debug

        newNode->next = (*ptr)->next;
        (*ptr)->next = newNode;
    }

    void SLList::Push(const char* d)
    {
        length++;
        Node* temp = new Node;
        const char** str = new const char*(d);
        temp->data = str;
        temp->next = first;

        if (first == nullptr)
        {
            last = temp;
        }

        first = temp;
    }

    const char* SLList::Peek()
    {
        if (first == nullptr)
        {
            return "";
        }
        return *(first->data);
    }

    const char* SLList::Pop()
    {
        if (first == nullptr)
        {
            return "";
        }
        length--;
        Node* temp = first;
        const char* deletedValue = *first->data;
        temp = first->next;
        delete first->data;
        delete first;
        first = temp;
        return deletedValue;
    }

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

    //make sure to delete[] it after use!
    const char** SLList::ToArray()
    {
        const char** result = new const char*[GetLength()];
        if (first != nullptr)
        {
            Node* temp = first;
            int count = 0;
            while (temp != nullptr)
            {
                result[count] = *temp->data;
                temp = temp->next;
                count++;
            }
        }
        return result;
    }

    bool SLList::Contains(std::string input)
    {
        bool result = false;
        Node* temp = first;
        while (temp != nullptr)
        {
            if (input == *temp->data)
            {
                result = true;
                break;
            }
            temp = temp->next;
        }
        return result;
    }

    //make sure to delete[] it after use!
    const char** SLList::ToCharArray() {
        const char** result = new const char*[GetLength()];
        if (first != nullptr) {
            Node* temp = first;
            int count = 0;
            while (temp != nullptr) {
                std::string d = *temp->data;
                result[count] = d.c_str();
                temp = temp->next;
                count++;
            }
        }
        return result;
    }
}