#include "backuphandler.h"

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

    void CheckBackup()
    {
        if (file.GetFilesInDirectory(config_path).size() > 0)
        {
            char a;
            std::cout << "Load last known backup? (y/n): ";
            std::cin >> a;

            if (a == 'y' || a == 'Y')
            {
                LoadBackup();
            }
        }
    }

    void BeginBackup()
    {
        std::time_t date = std::time(0);
        std::string date_str = std::to_string(date);
        std::string configFile = config_path + date_str + ".txt";

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
            content += controller.hashtables[i].GetAll() + "\n";
        }
        file.OverwriteFile(configStringNoSpaces, content);
    }

    void LoadBackup()
    {
        std::vector<std::string> files = file.GetFilesInDirectory(config_path);
        std::string filepath = config_path + files[0];
        std::ifstream file(filepath);
        std::string line;
        std::string id = "";
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
                id = line.substr(line.find("ID: ") + 4);
            }
            else
            {
                if (line.size() > 0)
                {
                    std::vector<std::string> elems;
                    std::stringstream ss(line);
                    std::string item;
                    while (std::getline(ss, item, ';'))
                    {
                        elems.push_back(item);
                    }

                    for (int i = 0; i < elems.size(); i++)
                    {
                        std::string value_id = elems[i];
                        std::string::size_type pos = value_id.find(":");
                        if (pos != std::string::npos)
                        {
                            value_id = value_id.substr(0, pos);
                        }
                        std::string value = elems[i].substr(elems[i].find(":") + 1);
                        controller.hashtables[atoi(id.c_str())].Add(value_id, value);
                    }
                }
            }
        }
    }
};
