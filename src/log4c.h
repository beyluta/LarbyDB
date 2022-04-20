#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include "file.h"

using std::cout;
using std::endl;
using std::string;

#define LOG(x) cout << x << endl;
#define LOG_PATH string(homedir) + "/LarbyDB/Logs/"

class Log4c
{
public:
    enum Color
    {
        WHITE = 97,
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        BLUE = 34,
        YELLOW = 33,
        MAGENTA = 34,
        CYAN = 36,
        LIGHT_GRAY = 37,
        DARK_GRAY = 90,
    };
    enum IOSystem
    {
        UNDEFINED = 0,
        TERMINAL = 1,
        FILESYSTEM = 2,
        BOTH = 3
    };
    void LogActivity(string log, IOSystem io = IOSystem::UNDEFINED, Color color = Color::WHITE);

private:
    string GetTime(const char *format = "%H:%M:%S");
    void LogToFile(string log);
    void LogToTerminal(string log, Color color = Color::WHITE);
};