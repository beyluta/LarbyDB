#include "main.h"
#include "hashtable.cpp"
#include <thread>
#include <chrono>

int main()
{
    Hashtable h = Hashtable();
    std::cout << h.Hash("Larry") << std::endl;
    h.Push("Larry");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    return 0;
}