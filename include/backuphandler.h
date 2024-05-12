#pragma once
#include "controller.h"
#include "file.h"
#include "jsonconverter.h"
#include <fstream>
#include <string>

class BackupHandler
{
public:
    Controller *m_controller;
    File m_file;
    std::string m_configPath;
    bool m_clearBackups = false;
    BackupHandler(Controller *controller, bool clear_backups);
    bool CheckBackup();
    void BeginBackup();
    void LoadBackup();
};
