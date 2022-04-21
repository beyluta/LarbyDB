#include "backuphandler.h"
#include "file.h"
#include "jsonconverter.h"
#include <fstream>
#include <string>

using namespace FileUtility;
using namespace BackupUtility;
using namespace JsonUtilities;

Controller *m_controller;
File m_file;
string m_configPath;
bool m_clearBackups = false;

BackupHandler::BackupHandler(Controller *controller, bool clearBackups)
{
    m_clearBackups = clearBackups;
    m_configPath = string(homedir) + "/LarbyDB/Backups/";
    m_controller = controller;
}

bool BackupHandler::CheckBackup()
{
    return m_file.GetFilesInDirectory(m_configPath).size() > 0 ? true : false;
}

void BackupHandler::BeginBackup()
{
    time_t date = time(0);
    string date_str = to_string(date);
    string configFile = m_configPath + date_str + ".txt";

    if (m_clearBackups)
    {
        m_file.DeleteDirectory(m_configPath, true);
    }

    string dbPath = string(homedir) + "/LarbyDB/";

    if (!m_file.DirectoryExists(dbPath))
    {
        m_file.CreateDirectory(dbPath);
    }

    string backupPath = dbPath + "Backups/";

    if (!m_file.DirectoryExists(backupPath))
    {
        m_file.CreateDirectory(backupPath);
    }

    string configStringNoSpaces = "";

    for (int i = 0; i < configFile.length(); i++)
    {
        if (configFile[i] != ' ' && configFile[i] != '\n')
        {
            configStringNoSpaces += configFile[i];
        }
    }

    m_file.CreateFile(configStringNoSpaces);

    string content;

    for (int i = 0; i < m_controller->GetSize(); i++)
    {
        content += "{\"id\":\"" + to_string(i) + "\",\"data\":" + m_controller->hashtables[i].GetAll() + "}\n";
    }

    m_file.OverwriteFile(configStringNoSpaces, content);
}

void BackupHandler::LoadBackup()
{
    vector<string> files = m_file.GetFilesInDirectory(m_configPath);
    string filepath = m_configPath + files[0];
    fstream file;

    file.open(filepath, ios::in);

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
                    string index = row["index"];
                    string value = row["value"];

                    if (m_controller->GetSize() <= atoi(table.c_str()))
                    {
                        m_controller->hashtables.resize(atoi(table.c_str()) + 1);
                    }

                    m_controller->hashtables[atoi(table.c_str())].AddTo(atoi(index.c_str()), value);
                }

                i++;
            }
        }

        file.close();
    }
}
