#include "main.h"
#include "hashtable.cpp"
#include "controller.cpp"

int main()
{
    // Hashtable ht;
    // ht.Add("Hello");
    // ht.Add("Larry");
    // ht.Add("Beyluta");
    // ht.PrintTable();

    Controller c(2);
    c.ResolveStringCommand("INSERT /<3 Larry/ INTO /0/ WHEREKEY /larry/");
    std::cout << c.ResolveStringCommand("FETCH /larry/ FROM /0/") << std::endl;
    return 0;
}