#pragma once
#include "controller.h"

using namespace ControllerUtility;

namespace BackupUtility
{
    class BackupHandler
    {
    public:
        BackupHandler(Controller *controller, bool clear_backups);
        bool CheckBackup();
        void BeginBackup();
        void LoadBackup();
    };
}