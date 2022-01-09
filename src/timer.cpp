#include "timer.h"

class Timer
{
private:
    std::vector<int (*)(int)> subscribbers;

    static void Task(int delay, std::vector<int (*)(int)> subs)
    {
        while (true)
        {
            sleep(delay);
            for (int i = 0; i < subs.size(); i++)
            {
                std::thread t(subs[i], 0);
                t.join();
            }
        }
    }

public:
    void Subscribe(int (*func)(int), int arg)
    {
        subscribbers.push_back(func);
    }

    void Start(int delay)
    {
        std::thread thread(Task, delay, subscribbers);
        thread.detach();
    }
};