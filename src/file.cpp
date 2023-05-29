#include "file.h"
#if __unix__ || __APPLE__
#include <dirent.h>
#elif _WIN32
#include <fileapi.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <string.h>
#include <sstream>
#include <fstream>

#if _WIN32
File::File()
{
    SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, homedir);
}

char *File::GetHomeDirectory()
{
    SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, homedir);
    return homedir;
}
#endif

void File::CreateFile(std::string filename)
{
    std::ofstream file;
    file.open(filename);
    file.close();
}

void File::MakeDirectory(std::string dirname)
{
#if _WIN32
    CreateDirectory(dirname.c_str(), nullptr);
#elif __unix__ ||  __APPLE__
    mkdir(dirname.c_str(), 0777);
#endif
}

void File::AppendLineToTextFile(std::string filename, std::string line)
{
    std::ifstream file;
    file.open(filename);
    std::stringstream stream;
    stream << file.rdbuf();
    OverwriteFile(filename, stream.str() + "\n" + line);
    file.close();
}

void File::OverwriteFile(std::string filename, std::string content)
{
    std::ofstream file;
    file.open(filename);
    file << content;
    file.close();
}

void File::DeleteFile(std::string filename)
{
    remove(filename.c_str());
}

void File::DeleteDirectory(std::string dirname, bool empty)
{
#if _WIN32
    if (empty)
    {
        WIN32_FIND_DATAA findData;
        HANDLE hFind = FindFirstFileA((dirname + "\\*").c_str(), &findData);

        if (hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
                if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
                {
                    std::string filepath = dirname + "\\" + findData.cFileName;
                    DeleteFileA(filepath.c_str());
                }
            } while (FindNextFileA(hFind, &findData));

            FindClose(hFind);
        }
    }

    RemoveDirectoryA(dirname.c_str());
#elif __unix__ || __APPLE__
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
#endif
}

bool File::FileExists(std::string filename)
{
    std::ifstream file(filename);
    return file.good();
}

bool File::DirectoryExists(std::string directory)
{
#if _WIN32
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((directory + "\\*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        FindClose(hFind);
        return true;
    }

    return false;
#elif __unix__ || __APPLE__
    DIR *dir;
    dir = opendir(directory.c_str());

    if (dir)
    {
        closedir(dir);
        return true;
    }
    return false;
#endif
}

std::vector<std::string> File::GetFilesInDirectory(std::string directory)
{
#if _WIN32
    std::vector<std::string> files;
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((directory + "\\*").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0)
            {
                files.push_back(findData.cFileName);
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);
    }

    return files;
#elif __unix__ || __APPLE__
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
#endif
}
