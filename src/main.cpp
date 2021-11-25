#define DEBUG
#include "linkedlist.cpp"
#include <iostream>

int main()
{
    std::cout << "Beyluta is sweet\n" << std::endl;
    LinkedList::SList myList;
    myList.AddToEnd("aaaaaaaaaaaaaaaaa");
    myList.AddToEnd("BBBBBBbbbbbbbbbbbbbbbbb");
#ifdef DEBUG
    myList.PrintLIst();
#endif
    return 0;
}