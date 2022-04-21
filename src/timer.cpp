#include "timer.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

std::vector<int (*)(int)> subscribbers;

void Timer::Task(int delay, std::vector<int (*)(int)> subs)
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(delay));
        for (int i = 0; i < subs.size(); i++)
        {
            std::thread t(subs[i], 0);
            t.join();
        }
    }
}

void Timer::Subscribe(int (*func)(int), int arg)
{
    subscribbers.push_back(func);
}

void Timer::Start(int delay)
{
    std::thread thread(Task, delay, subscribbers);
    thread.detach();
}
