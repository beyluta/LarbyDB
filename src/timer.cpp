#include "timer.h"

class Timer
{
private:
    static std::vector<int (*)(int)> subscribbers;

    static void Task(int delay)
    {
        while (true)
        {
            std::cout << "Timer task" << std::endl;
            sleep(delay);
        }
    }

    static void AlertSubscribbers()
    {
        for (int i = 0; i < subscribbers.size(); i++)
        {
            std::thread t(subscribbers[i], 0);
            t.join();
        }
    }

public:
    void Subscribe(int (*func)(int), int arg)
    {
        std::cout << "Subscribbed" << std::endl;
        subscribbers.push_back(func);
    }

    void Start(int delay)
    {
        std::thread thread(Task, delay);
        thread.detach();
    }
};