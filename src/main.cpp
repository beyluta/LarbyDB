#include "main.h"
#include "hashtable.cpp"
#include <thread>
#include <chrono>



int main()
{
    Hashtable h = Hashtable();
    h.Push("Larry");

    
    // std::cout << "Beyluta is sweet\n" << std::endl;
    // LinkedList::SLList myList;
    // myList.Push("4");
    // myList.Push("3");
    // myList.Push("2");
    // myList.Push("1");
    // myList.AddToEnd("5");
    // myList.Insert("inseted element", 3);
    // std::cout << myList.GetLength() << '\n';
    // std::cout << myList.Pop() << '\n';
    // std::cout << myList.GetLength() << '\n';
    // myList.PrintList();
    // std::string* myArr = myList.ToArray();

    // for (int i = 0; i < myList.GetLength(); i++)
    //     std::cout << myArr[i] << '\t';
    // std::cout << '\n';
    // myList.Pop();
    // myList.Push("new 1");
    // myList.AddToEnd("new 2");
    // delete[] myArr;
    // myArr = myList.ToArray();

    // for (int i = 0; i < myList.GetLength(); i++)
    //     std::cout << myArr[i] << '\t';
    // std::cout << '\n';
    // delete[] myArr;

    return 0;
}