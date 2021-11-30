#include "hashtable.h"

class Hashtable
{
private:
    SLList *arr;
    int size = 0;

public:
    Hashtable()
    {
        arr = new SLList[0];
    }

    int Hash(const char *value)
    {
        int sum = 0;
        for (int i = 0; i < strlen(value); i++)
        {
            sum += (int)value[i];
        }
        return sum;
    }

    void Push(const char *value)
    {
        int hash = Hash(value);

        if (hash > size)
        {
            std::cout << "Expanded array to " << hash << std::endl;
            SLList *newArr = new SLList[hash];
            for (int i = 0; i < size; i++)
            {
                auto content = arr[i].ToArray();
                for (int j = 0; j < arr[i].GetLength(); j++)
                {
                    newArr[i].Push(content[j]);
                    std::cout << "pushed" << content[i] << " into the array" << std::endl;
                }
            }
            arr = newArr;
            size = hash;
        }

        std::cout << "add " << value << std::endl;
        arr[hash].Push(value);
    }

    void PrintAllTables()
    {
        for (int i = 0; i < size; i++)
        {
            if (arr[i].GetLength() > 0)
            {
                std::cout << "----Table " << i << " with length " << arr[i].GetLength() << "----" << std::endl;
                auto content = arr[i].ToArray();
                for (int j = 0; j < arr[i].GetLength(); j++)
                {
                    std::cout << content[j] << std::endl;
                }
            }
        }
    }
};