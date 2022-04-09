#include "backuphandler.h"

class BackupHandler
{
private:
    Controller *controller;
    File file;
    string config_path;
    bool clear_backups = false;

public:
    BackupHandler(Controller *controller, bool clear_backups)
    {
        this->clear_backups = clear_backups;
        config_path = string(homedir) + "/LarbyDB/Backups/";
        this->controller = controller;
    }

    bool CheckBackup()
    {
        return file.GetFilesInDirectory(config_path).size() > 0 ? true : false;
    }

    void BeginBackup()
    {
        time_t date = time(0);
        string date_str = to_string(date);
        string configFile = config_path + date_str + ".txt";

        if (clear_backups)
        {
            file.DeleteDirectory(config_path, true);
        }

        string dbPath = string(homedir) + "/LarbyDB/";

        if (!file.DirectoryExists(dbPath))
        {
            file.CreateDirectory(dbPath);
        }

        string backupPath = dbPath + "Backups/";

        if (!file.DirectoryExists(backupPath))
        {
            file.CreateDirectory(backupPath);
        }

        string configStringNoSpaces = "";

        for (int i = 0; i < configFile.length(); i++)
        {
            if (configFile[i] != ' ' && configFile[i] != '\n')
            {
                configStringNoSpaces += configFile[i];
            }
        }

        file.CreateFile(configStringNoSpaces);

        string content;

        for (int i = 0; i < controller->GetSize(); i++)
        {
            content += "{\"id\":\"" + to_string(i) + "\",\"data\":" + controller->hashtables[i].GetAll() + "}\n";
        }

        file.OverwriteFile(configStringNoSpaces, content);
    }

    void LoadBackup()
    {
        vector<string> files = file.GetFilesInDirectory(config_path);
        string filepath = config_path + files[0];
        ifstream file(filepath);
        string line;

        while (getline(file, line))
        {
            string table = GetJSONFieldValues(line, "id");
            string objectString = GetJSONFieldValues(line, "data");
            vector<string> parsedObjects = SplitJSONStringObjects(objectString);

            if (objectString.length() > 0)
            {
                for (int i = 0; i < parsedObjects.size(); i += 2)
                {
                    string key = GetJSONFieldValues(parsedObjects[i], "index");
                    string value = GetJSONFieldValues(parsedObjects[i + 1], "value");

                    if (controller->GetSize() <= atoi(table.c_str()))
                    {
                        controller->hashtables.resize(atoi(table.c_str()) + 1);
                    }

                    controller->hashtables[atoi(table.c_str())].AddTo(atoi(key.c_str()), value);
                }
            }
        }
    }
};
