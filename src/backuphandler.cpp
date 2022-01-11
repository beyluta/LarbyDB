#include "backuphandler.h"

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
};

class BackupHandler
{
private:
    File file;

public:
    void BeginBackup(Controller &c)
    {
        std::string configPath = std::string(homedir) + "/LarbyDB/Backups/";
        std::time_t date = std::time(0);
        std::string configFile = configPath + ctime(&date) + ".txt";

        std::string larbydb_path = std::string(homedir) + "/LarbyDB/";
        if (!file.DirectoryExists(larbydb_path))
        {
            file.CreateDirectory(larbydb_path);
        }

        std::string backup_path = larbydb_path + "Backups/";
        if (!file.DirectoryExists(backup_path))
        {
            file.CreateDirectory(backup_path);
        }

        std::string configStringNoSpaces = "";
        for (int i = 0; i < configFile.length(); i++)
        {
            if (configFile[i] != ' ' && configFile[i] != '\n')
            {
                configStringNoSpaces += configFile[i];
            }
        }
        //file.CreateFile(configStringNoSpaces);

        std::string content = "";
        for (int i = 0; i < c.GetSize(); i++)
        {
            content += "ID: " + to_string(i) + "\n";
            content += c.ResolveStringCommand("FETCH /*/ FROM /" + std::to_string(i) + "/") + "\n";
        }
        std::cout << content << std::endl;
    }
};
