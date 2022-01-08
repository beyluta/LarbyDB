#include "main.h"
#include "hashtable.cpp"
#include "Controller.cpp"

int main()
{
    // Hashtable ht;
    // ht.Add("Hello");
    // ht.Add("Larry");
    // ht.Add("Beyluta");
    // ht.PrintTable();

    Controller c(2);
    c.ResolveStringCommand("INSERT /Hello/ INTO /0/");
    c.ResolveStringCommand("INSERT /Hello2 /INTO /   1   /");
    c.ResolveStringCommand("DELETE /Hello/ FROM /0/");
    c.hashtables[0].PrintTable(0);
    c.hashtables[1].PrintTable(1);
    return 0;
}