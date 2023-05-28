#include "logsys.h"
#include <iostream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <iomanip>
#include "file.h"

using namespace Logsys;
File file;

#ifdef _WIN32
std::string LOG_PATH = std::string(file.GetHomeDirectory()) + "\\LarbyDB\\Logs\\";
#endif

string Logsys::GetTime(const char *format)
{
    using namespace std::chrono;

    auto now = system_clock::now();
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    auto timer = system_clock::to_time_t(now);
    tm bt = *localtime(&timer);
    std::ostringstream oss;

    oss << std::put_time(&bt, format);
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

void Logsys::LogToFile(string log)
{
    string path = LOG_PATH + "log.txt";

#ifdef _WIN32
    if (!file.DirectoryExists(std::string(file.GetHomeDirectory()) + "\\LarbyDB"))
    {
        file.MakeDirectory(std::string(file.GetHomeDirectory()) + "\\LarbyDB");
    }
#endif

    if (!file.DirectoryExists(LOG_PATH))
    {
        file.MakeDirectory(LOG_PATH);
    }

    if (!file.FileExists(path))
    {
        file.CreateFile(path);
    }

    file.AppendLineToTextFile(path, log);
}

void Logsys::LogToTerminal(string log, Logsys::Color color)
{
    string code = "\033[0;" + std::to_string((int)color) + ";49m" + log + "\033[0m";
    LOG(code);
}

void Logsys::LogActivity(string log, Logsys::IOSystem io, Color color, bool showTime)
{
    if (showTime)
    {
        log = GetTime() + ": " + log;
    }

    switch (io)
    {
    case Logsys::IOSystem::TERMINAL:
        LogToTerminal(log, color);
        break;

    case Logsys::IOSystem::FILESYSTEM:
        LogToFile(log);
        break;

    case Logsys::IOSystem::BOTH:
        LogToTerminal(log, color);
        LogToFile(log);
        break;

    case Logsys::IOSystem::UNDEFINED:
        LogToTerminal("Undefined IO System when calling " + string(__func__), Logsys::Color::RED);
        break;
    }
}