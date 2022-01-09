#include "main.h"

class BackupHandler
{
public:
    void BeginBackup(Controller &c)
    {
        std::cout << "Backup started" << std::endl;
        c.ResolveStringCommand("Test");
    }
};