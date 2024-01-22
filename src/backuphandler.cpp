#include "backuphandler.h"

BackupHandler::BackupHandler(Controller *controller, bool clearBackups)
{
    BackupHandler::m_clearBackups = clearBackups;
#if _WIN32
    BackupHandler::m_configPath = std::string(m_file.GetHomeDirectory()) + "\\LarbyDB\\Backups\\";
#elif __unix__ || __APPLE__
    BackupHandler::m_configPath = std::string(homedir) + "/LarbyDB/Backups/";
#endif

    BackupHandler::m_controller = controller;
}

bool BackupHandler::CheckBackup()
{
    return BackupHandler::m_file.GetFilesInDirectory(BackupHandler::m_configPath).size() > 0 ? true : false;
}

void BackupHandler::BeginBackup()
{
    time_t date = time(0);
    std::string date_str = std::to_string(date);
    std::string configFile = BackupHandler::m_configPath + date_str + ".txt";

    if (m_clearBackups)
    {
        BackupHandler::m_file.DeleteDirectory(BackupHandler::m_configPath, true);
    }

#if _WIN32
    std::string dbPath = std::string(m_file.GetHomeDirectory()) + "\\LarbyDB\\";
#elif __unix__ || __APPLE__
    std::string dbPath = std::string(homedir) + "/LarbyDB/";
#endif

    if (!BackupHandler::m_file.DirectoryExists(dbPath))
    {
        BackupHandler::m_file.MakeDirectory(dbPath);
    }

#if _WIN32
    std::string backupPath = std::string(m_file.GetHomeDirectory()) + "\\LarbyDB\\Backups\\";
#elif __unix__ || __APPLE__
    std::string backupPath = dbPath + "Backups/";
#endif

    if (!BackupHandler::m_file.DirectoryExists(backupPath))
    {
        BackupHandler::m_file.MakeDirectory(backupPath);
    }

    std::string configStringNoSpaces = "";

    for (int i = 0; i < configFile.length(); i++)
    {
        if (configFile[i] != ' ' && configFile[i] != '\n' && configFile[i] != '\r')
        {
            configStringNoSpaces += configFile[i];
        }
    }

    BackupHandler::m_file.CreateFile(configStringNoSpaces);

    std::string content;

    for (int i = 0; i < BackupHandler::m_controller->GetSize(); i++)
    {
        std::string values = BackupHandler::m_controller->hashtables[i].GetAll();
#if _WIN32
        values.erase(std::remove(values.begin(), values.end(), '\n'), values.end());
        values.erase(std::remove(values.begin(), values.end(), '\r'), values.end());
#endif
        content += "{\"id\":\"" + std::to_string(i) + "\",\"data\":" + values + "}\n";
    }

    BackupHandler::m_file.OverwriteFile(configStringNoSpaces, content);
}

void BackupHandler::LoadBackup()
{
    std::vector<std::string> files = m_file.GetFilesInDirectory(m_configPath);
    std::string filepath = m_configPath + files[0];
    std::fstream file;

    file.open(filepath, std::ios::in);

    if (file.is_open())
    {
        string line;

        while (getline(file, line))
        {
            Json json(line);
            string table = json["id"];
            Json data = json["data"];
            bool endOfData = false;
            int i = 0;

            while (!endOfData)
            {
                if (data[i] == "")
                {
                    endOfData = true;
                }
                else
                {
                    Json row = data[i];
                    std::string index = row["index"];
                    std::string value = row["value"];

                    if (BackupHandler::m_controller->GetSize() <= atoi(table.c_str()))
                    {
                        BackupHandler::m_controller->hashtables.resize(atoi(table.c_str()) + 1);
                    }

                    if (json.IsArray(value))
                    {
                        int count = json.GetSize(value);

                        for (int i = 0; i < count; i++)
                        {
                            std::string object = json.GetObjectFromJsonArray(value, i);
                            BackupHandler::m_controller->hashtables[atoi(table.c_str())].AddTo(atoi(index.c_str()), object);
                        }
                    }
                    else
                    {
                        BackupHandler::m_controller->hashtables[atoi(table.c_str())].AddTo(atoi(index.c_str()), value);
                    }
                }

                i++;
            }
        }

        file.close();
    }
}
