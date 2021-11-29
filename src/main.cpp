#include "main.h"



int main()
{
    std::cout << "Beyluta is sweet\n" << std::endl;
    LinkedList::SLList myList;
    myList.Push("4");
    myList.Push("3");
    myList.Push("2");
    myList.Push("1");
    myList.AddToEnd("5");
    myList.Insert("inseted element", 3);
    std::cout << myList.GetLength() << '\n';
    std::cout << myList.Pop() << '\n';
    std::cout << myList.GetLength() << '\n';
    myList.PrintList();
    myList.Pop();
    std::cout << myList.Contains("new 1") << '\n';
    myList.Push("new 1");
    myList.AddToEnd("new 2");
    myList.PrintList();
    std::cout << myList.Contains("new 1") << '\n';

    return 0;
}