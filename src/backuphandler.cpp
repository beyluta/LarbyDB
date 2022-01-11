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

class BackupHandler
{
private:
    File file;
    std::string config_path;
    bool clear_backups = false;
    Controller controller;

public:
    BackupHandler(Controller &controller, bool clear_backups)
    {
        this->clear_backups = clear_backups;
        this->config_path = std::string(homedir) + "/LarbyDB/Backups/";
        this->controller = controller;
    }

    void BeginBackup()
    {
        std::time_t date = std::time(0);
        std::string configFile = config_path + ctime(&date) + ".txt";

        if (clear_backups)
        {
            file.DeleteDirectory(config_path, true);
        }

        std::string dbPath = std::string(homedir) + "/LarbyDB/";
        if (!file.DirectoryExists(dbPath))
        {
            file.CreateDirectory(dbPath);
        }

        std::string backupPath = dbPath + "Backups/";
        if (!file.DirectoryExists(backupPath))
        {
            file.CreateDirectory(backupPath);
        }

        std::string configStringNoSpaces = "";
        for (int i = 0; i < configFile.length(); i++)
        {
            if (configFile[i] != ' ' && configFile[i] != '\n')
            {
                configStringNoSpaces += configFile[i];
            }
        }
        file.CreateFile(configStringNoSpaces);

        std::string content = "";
        for (int i = 0; i < controller.GetSize(); i++)
        {
            content += "ID: " + to_string(i) + "\n";
            content += controller.ResolveStringCommand("FETCH /*/ FROM /" + std::to_string(i) + "/") + "\n";
        }
        file.OverwriteFile(configStringNoSpaces, content);
    }

    void LoadBackup()
    {
        std::vector<std::string> files = file.GetFilesInDirectory(config_path);
        std::string filepath = config_path + files[0];
        std::ifstream file(filepath);
        std::string line;
        while (std::getline(file, line))
        {
            int string_before_id = 0;
            std::string::size_type pos = line.find("ID: ");
            if (pos != std::string::npos)
            {
                string_before_id = line.substr(0, pos).size();
            }

            if (line.find("ID: ") != std::string::npos && std::all_of(line.begin() + 4, line.end(), ::isdigit) && string_before_id <= 0)
            {
                std::string id = line.substr(line.find("ID: ") + 4);
            }
            else
            {
                std::cout << line << std::endl;
            }
            sleep(1);
        }
    }
};
