#include "main.h"
#include "hashtable.cpp"
#include <thread>
#include <chrono>



int main()
{
    // Hashtable h = Hashtable();
    // h.Push("Larry");
    // h.Push("Beyluta");
    // h.Push("Micha");
    // h.Push("Sebastian");
    // h.Push("Patrick");
    // h.PrintAllTables();

    SLList* l = new SLList[11];
    l[2].Push("Beyluta");
    l[2].Push("Larry");
    l[6].Push("Sebastian");
    l[6].Push("Micha");
    l[10].Push("Patrick");
    l[10].Push("Burak");
    l[2].PrintList();

    auto x = l[2].ToArray();
    std::cout << x[0] << " " << x[1] << '\n';

    delete[] x;
    delete[] l;

    
    // for(int i=0;i<11; i++){
    //     std::cout<<l[i].GetLength()<<std::endl;
    //     std::string* x = l[i].ToArray();
    //     delete[] x;
    //     // auto content = l[i].ToArray();
    //     // for (int j=0;j<l[i].GetLength();j++){
    //     //     std::cout<<content[j]<<std::endl;
    //     // }
    // }
   

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