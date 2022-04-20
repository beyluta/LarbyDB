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
private:
    string GetTime(const char *format = "%H:%M:%S");
    void LogToFile(string log);

public:
    enum IOSystem
    {
        UNDEFINED = 0,
        TERMINAL = 1,
        FILESYSTEM = 2,
        BOTH = 3
    };
    void LogActivity(string log, IOSystem io = IOSystem::UNDEFINED);
};