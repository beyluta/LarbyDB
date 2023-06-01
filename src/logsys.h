#pragma once
#include <iostream>

using std::cout;
using std::endl;
using std::string;

#define LOG(x) cout << x << endl;
#if __unix__ || __APPLE__
#define LOG_PATH string(homedir) + "/LarbyDB/Logs/"
#endif

namespace Logsys
{
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
    string GetTime(const char *format = "%H:%M:%S");
    void LogToFile(string log);
    void LogToTerminal(string log, Logsys::Color color);
    void LogActivity(string log, IOSystem io = IOSystem::UNDEFINED, Color color = Color::WHITE, bool showTime = true);
};