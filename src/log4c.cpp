#include "log4c.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include "file.h"

using namespace FileUtility;

File file;

string Log4c::GetTime(const char *format)
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = system_clock::to_time_t(now);
    tm bt = *localtime(&timer);
    ostringstream oss;

    oss << put_time(&bt, format);
    oss << '.' << setfill('0') << setw(3) << ms.count();
    return oss.str();
}

void Log4c::LogToFile(string log)
{
    string path = LOG_PATH + "log.txt";

    if (!file.DirectoryExists(LOG_PATH))
    {
        file.CreateDirectory(LOG_PATH);
    }

    if (!file.FileExists(path))
    {
        file.CreateFile(path);
    }

    file.AppendLineToTextFile(path, log);
}

void Log4c::LogToTerminal(string log, Log4c::Color color)
{
    string code = "\033[0;" + to_string((int)color) + ";49m" + log + "\033[0m";
    LOG(code);
}

void Log4c::LogActivity(string log, Log4c::IOSystem io, Color color)
{
    log = GetTime() + ": " + log;

    switch (io)
    {
    case Log4c::IOSystem::TERMINAL:
        LogToTerminal(log, color);
        break;

    case Log4c::IOSystem::FILESYSTEM:
        LogToFile(log);
        break;

    case Log4c::IOSystem::BOTH:
        LogToTerminal(log, color);
        LogToFile(log);
        break;

    case Log4c::IOSystem::UNDEFINED:
        LogToTerminal("Undefined IO System when calling " + string(__func__), Log4c::Color::RED);
        break;
    }
}