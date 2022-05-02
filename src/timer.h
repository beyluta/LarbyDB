#pragma once
#include <vector>
#include <functional>

class Timer
{
private:
    void Task(int delay, std::vector<int (*)(int)> subs);

public:
    void Subscribe(std::function<int(int)>, int delay);
    void Start();
};
