#include "log4c.h"

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

void Log4c::LogActivity(string log, Log4c::IOSystem io)
{
    log = GetTime() + ": " + log;

    switch (io)
    {
    case Log4c::IOSystem::TERMINAL:
        LOG(log);
        break;

    case Log4c::IOSystem::FILESYSTEM:
        LogToFile(log);
        break;

    case Log4c::IOSystem::BOTH:
        LOG(log);
        LogToFile(log);
        break;

    case Log4c::IOSystem::UNDEFINED:
        LOG("Undefined IO System when calling Log4C::LogActivity");
        break;
    }
}