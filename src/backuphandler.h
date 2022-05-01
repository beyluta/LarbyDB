#pragma once
#include "controller.h"

class BackupHandler
{
public:
    BackupHandler(Controller *controller, bool clear_backups);
    bool CheckBackup();
    void BeginBackup();
    void LoadBackup();
};
