#include "timer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

struct Task
{
    int delay;
    std::function<int(int)> function;
};

std::vector<Task> tasks;

void Timer::Task(int delay, std::function<int(int)> func)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        std::thread t(func, 0);
        t.join();

        // for (int i = 0; i < func.size(); i++)
        // {
        //     std::thread t(func[i], 0);
        //     t.join();
        // }
    }
}

void Timer::Subscribe(std::function<int(int)> func, int delay)
{
    tasks.push_back({delay, func});
}

void Timer::Start()
{
    // std::thread thread(Task, delay, subscribbers);
    // thread.detach();

    for (int i = 0; i < tasks.size(); i++)
    {
        std::thread t(Task, tasks[i].delay, tasks[i].function);
        t.detach();
    }
}
