#include <cstring>
using namespace LinkedList;

int Hash(const char *value)
{
    int sum = 0;
    for (int i = 0; i < strlen(value); i++)
    {
        sum += (int)value[i];
    }
    return sum;
}