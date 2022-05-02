#pragma once
#include <vector>
#include <functional>

class Timer
{
private:
    static void Task(int delay, std::function<int(int)> func);

public:
    void Subscribe(std::function<int(int)>, int delay);
    void Start();
};
