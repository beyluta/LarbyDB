#define DEBUG
//#define MEM_DEBUG
#include "linkedlist.cpp"
#include <iostream>

#ifdef MEM_DEBUG
struct MemoryAllocationMetrics
{
    uint32_t allocated = 0;
    uint32_t freed = 0;

    uint32_t InUse()
    {
        return allocated - freed;
    }
};

static MemoryAllocationMetrics _memAllocationMetrics;

void PrintMemoryUsage();

void* operator new(size_t size)
{
    std::cout << "allocated " << size << " bytes\n";
    _memAllocationMetrics.allocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    std::cout << "free() was called, deleted " << size << " bytes. ";
    _memAllocationMetrics.freed += size;
    PrintMemoryUsage();
    free(memory);
}
#endif

void PrintMemoryUsage()
{
#ifdef MEM_DEBUG
    std::cout << "Currently using " << _memAllocationMetrics.InUse() << " bytes\n";
#endif
}


int main()
{
    PrintMemoryUsage();
    std::cout << "Beyluta is sweet\n" << std::endl;
    LinkedList::SLList myList;
    std::string test;
    std::cin >> test;
    myList.AddToEnd(test);
#ifdef DEBUG
    myList.PrintList();
    test.~basic_string();
    std::cin >> test;
    myList.AddToEnd(test);
    myList.PrintList();
    std::cout << myList.Peek() << '\n';
    std::cout << "popped " << myList.Pop() << '\n';
    std::cout << myList.Peek() << '\n';
    myList.PrintList();
#endif
    return 0;
}