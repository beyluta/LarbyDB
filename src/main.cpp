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

    Controller c;
    c.ResolveStringCommand("INSERT /Hello/ INTO /0/");
    return 0;
}