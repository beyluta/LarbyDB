#include "controller.h"

int Hash(const char *value);

class Controller
{
private:
    Hashtable **db;
    int size = 0;

public:
    Controller(int size)
    {
        db = new Hashtable *[size];
        for (int i = 0; i < size; i++)
        {
            db[i] = new Hashtable();
        }
        Controller::size = size;
    }

    ~Controller()
    {
        for (int i = 0; i < size; i++)
        {
            delete db[i];
        }
        delete[] db;
    }

    void AddToTableById(int id, const char *value)
    {
        if (id < size)
        {
            if (!db[id]->Contains(value))
            {
                db[id]->Push(value);
            }
        }
    }

    void AddKeyValueToTableById(int id, const char *key, const char *value)
    {
        if (id < size)
        {
            if (!db[id]->Contains(value))
            {
                int hash = Hash(key);
                std::cout << hash << std::endl;
                // db[id]->Push(value);
            }
        }
    }

    bool ContainsInTableById(int id, const char *value)
    {
        if (id < size)
        {
            return db[id]->Contains(value);
        }
        return false;
    }
};