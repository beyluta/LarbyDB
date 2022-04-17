#pragma once
#include "file.h"
#include "jsonconverter.h"
#include <fstream>
#include <string>
#include "controller.h"

using namespace JsonUtilities;
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