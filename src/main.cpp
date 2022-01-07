#include "main.h"
#include "hashtable.cpp"

int main()
{
    Hashtable ht;
    ht.Add("Hello");
    ht.Add("Larry");
    ht.Add("Beyluta");
    ht.PrintTable();
    return 0;
}