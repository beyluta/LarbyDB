#include "main.h"
#define MEM_DEBUG

#ifdef MEM_DEBUG

unsigned int memory_in_use = 0;

#define PRINT_MEMORY_USAGE std::cout << memory_in_use << " bytes currently in use\n";

void* operator new(size_t size)
{
    //std::cout << "allocated " << size << " bytes\n";
    //std::cout << memory_in_use << '\n';
    memory_in_use+= size;
    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    //std::cout << "freed " << size << "bytes\n";
    //std::cout << memory_in_use << '\n';
    PRINT_MEMORY_USAGE
    memory_in_use-= size;
    free(memory);
}
#else
#define PRINT_MEMORY_USAGE
#endif



int main()
{
    std::cout << "Beyluta is sweet\n" << std::endl;

    PRINT_MEMORY_USAGE

    LinkedList::SLList* myList = new LinkedList::SLList;
    PRINT_MEMORY_USAGE
    myList->Insert("inserted 0", 0);
    PRINT_MEMORY_USAGE
    myList->Push("3");
    PRINT_MEMORY_USAGE
    myList->Push("2");
    PRINT_MEMORY_USAGE
    myList->Push("1");
    PRINT_MEMORY_USAGE
    myList->AddToEnd("4");
    PRINT_MEMORY_USAGE
    myList->AddToEnd("5");
    PRINT_MEMORY_USAGE
    myList->AddToEnd("6");
    PRINT_MEMORY_USAGE
    myList->Insert("inserted 1", 0);
    PRINT_MEMORY_USAGE
    myList->Insert("inserted 2", 69);
    PRINT_MEMORY_USAGE
    myList->Insert("inserted 3", 8);
    PRINT_MEMORY_USAGE
    myList->Insert("inserted 4", 8);
    PRINT_MEMORY_USAGE
    myList->Insert("inserted 5", 3);
    PRINT_MEMORY_USAGE
    myList->PrintList();
    auto arr = myList->ToCharArray();
    PRINT_MEMORY_USAGE
    for (int i = 0; i < myList->GetLength(); i++)
        std::cout << '"' << *(&arr[i]) << "\", ";
    std::cout << '\n';
    delete[] arr;
    PRINT_MEMORY_USAGE
    delete myList;
    PRINT_MEMORY_USAGE
    return 0;
}