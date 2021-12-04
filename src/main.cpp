#include "main.h"
#include <thread>
#include <chrono>

int main()
{
    std::cout << "Beyluta is sweet\n" << std::endl;

    LinkedList::SLList* myList = new LinkedList::SLList;
    myList->Insert("inserted 0", 0);
    myList->Push("3");
    myList->Push("2");
    myList->Push("1");
    myList->AddToEnd("4");
    myList->AddToEnd("5");
    myList->AddToEnd("6");
    myList->Insert("inserted 1", 0);
    myList->Insert("inserted 2", 69);
    myList->Insert("inserted 5", 3);
    myList->PrintList();
    auto arr = myList->ToArray();
    for (int i = 0; i < myList->GetLength(); i++)
        std::cout << '"' << arr[i] << "\", ";
    std::cout << '\n';
    arr[0] = "changed"; // do not do this
    myList->PrintList();

    for (int i = 0; i < myList->GetLength(); i++)
        std::cout << '"' << arr[i] << "\", ";
    std::cout << '\n';

    delete[] arr;
    std::cout << myList->Contains("inserted 5") << " " << myList->Contains("no") << '\n';
    delete myList;
    return 0;
}