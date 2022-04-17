#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>
#include <vector>

namespace TimerUtility
{
    class Timer
    {
    private:
        static void Task(int delay, std::vector<int (*)(int)> subs);

    public:
        void Subscribe(int (*func)(int), int arg);
        void Start(int delay);
    };
}