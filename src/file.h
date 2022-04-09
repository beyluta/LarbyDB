#pragma once
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "main.h"
#include <dirent.h>
#include <sys/stat.h>
#include <algorithm>

struct passwd *pw = getpwuid(getuid());
const char *homedir = pw->pw_dir;

class File
{
public:
    void CreateFile(std::string filename)
    {
        std::ofstream file;
        file.open(filename);
        file.close();
    }

    void CreateDirectory(std::string dirname)
    {
        mkdir(dirname.c_str(), 0777);
    }

    void OverwriteFile(std::string filename, std::string content)
    {
        std::ofstream file;
        file.open(filename);
        file << content;
        file.close();
    }

    void DeleteFile(std::string filename)
    {
        remove(filename.c_str());
    }

    void DeleteDirectory(std::string dirname, bool empty)
    {
        if (empty)
        {
            DIR *dir;
            struct dirent *ent;

            if ((dir = opendir(dirname.c_str())) != NULL)
            {
                while ((ent = readdir(dir)) != NULL)
                {
                    if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                    {
                        std::string filepath = dirname + "/" + ent->d_name;
                        DeleteFile(filepath);
                    }
                }
                closedir(dir);
            }
        }
        rmdir(dirname.c_str());
    }

    bool FileExists(std::string filename)
    {
        std::ifstream file(filename);
        return file.good();
    }

    bool DirectoryExists(std::string directory)
    {
        DIR *dir;
        dir = opendir(directory.c_str());

        if (dir)
        {
            closedir(dir);
            return true;
        }
        return false;
    }

    std::vector<std::string> GetFilesInDirectory(std::string directory)
    {
        std::vector<std::string> files;
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir(directory.c_str())) != NULL)
        {
            while ((ent = readdir(dir)) != NULL)
            {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
                {
                    files.push_back(ent->d_name);
                }
            }
            closedir(dir);
        }
        return files;
    }
};