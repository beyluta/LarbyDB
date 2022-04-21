#include "file.h"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <algorithm>
#include <string.h>
#include <sstream>
#include <fstream>

void File::CreateFile(string filename)
{
    ofstream file;
    file.open(filename);
    file.close();
}

void File::CreateDirectory(string dirname)
{
    mkdir(dirname.c_str(), 0777);
}

void File::AppendLineToTextFile(string filename, string line)
{
    ifstream file;
    file.open(filename);
    stringstream stream;
    stream << file.rdbuf();
    OverwriteFile(filename, stream.str() + "\n" + line);
    file.close();
}

void File::OverwriteFile(string filename, string content)
{
    ofstream file;
    file.open(filename);
    file << content;
    file.close();
}

void File::DeleteFile(string filename)
{
    remove(filename.c_str());
}

void File::DeleteDirectory(string dirname, bool empty)
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
                    string filepath = dirname + "/" + ent->d_name;
                    DeleteFile(filepath);
                }
            }
            closedir(dir);
        }
    }
    rmdir(dirname.c_str());
}

bool File::FileExists(string filename)
{
    ifstream file(filename);
    return file.good();
}

bool File::DirectoryExists(string directory)
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

vector<string> File::GetFilesInDirectory(string directory)
{
    vector<string> files;
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
