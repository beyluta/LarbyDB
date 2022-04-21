#pragma once
#include <vector>

class Timer
{
private:
    static void Task(int delay, std::vector<int (*)(int)> subs);

public:
    void Subscribe(int (*func)(int), int arg);
    void Start(int delay);
};
