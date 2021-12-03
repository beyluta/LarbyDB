#include "controller.h"

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
                std::cout << "Pushed " << value << " to table " << id << std::endl;
                db[id]->Push(value);
            }
        }
    }
};