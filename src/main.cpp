#define DEBUG 1
#include <iostream>
#include "linkedlist.h"

int main()
{
    std::cout << "Beyluta is sweet\n" << std::endl;
    LinkedList::LinkedList myList;
    myList.AddToEnd("aaaaaaaaaaaaaaaaa");
#ifdef DEBUG
    myList.PrintLIst();
#endif
    return 0;
}