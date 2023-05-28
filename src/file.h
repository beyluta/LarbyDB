#pragma once
#include <iostream>
#include <vector>
#ifdef __unix__
#include <pwd.h>
#include <unistd.h>
#elif _WIN32
#include <ShlObj.h>
#include <Windows.h>
#endif

#ifdef __unix__
static struct passwd *pw = getpwuid(getuid());
static const char *homedir = pw->pw_dir;
#elif _WIN32
static char homedir[MAX_PATH];
#endif

class File
{
public:
#ifdef _WIN32
    File();
    char *GetHomeDirectory();
#endif
    void CreateFile(std::string filename);
    void MakeDirectory(std::string dirname);
    void AppendLineToTextFile(std::string filename, std::string line);
    void OverwriteFile(std::string filename, std::string content);
    void DeleteFile(std::string filename);
    void DeleteDirectory(std::string dirname, bool empty);
    bool FileExists(std::string filename);
    bool DirectoryExists(std::string directory);
    std::vector<std::string> GetFilesInDirectory(std::string directory);
};
