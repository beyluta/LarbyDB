#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>
#include <vector>
#include <string.h>

using namespace std;

static struct passwd *pw = getpwuid(getuid());
static const char *homedir = pw->pw_dir;

namespace FileUtility
{
    class File
    {
    public:
        void CreateFile(string filename);
        void CreateDirectory(string dirname);
        void AppendLineToTextFile(string filename, string line);
        void OverwriteFile(string filename, string content);
        void DeleteFile(string filename);
        void DeleteDirectory(string dirname, bool empty);
        bool FileExists(string filename);
        bool DirectoryExists(string directory);
        vector<string> GetFilesInDirectory(string directory);
    };
}