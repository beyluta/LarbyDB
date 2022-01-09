#include "backuphandler.h"

class BackupHandler
{
public:
    void BeginBackup(Controller &c)
    {
        std::string configPath = std::string(homedir) + "/LarbyDB/cfg/";
        cout << configPath << endl;
        for (int i = 0; i < c.GetSize(); i++)
        {
            c.ResolveStringCommand("FETCH /*/ FROM /" + std::to_string(i) + "/");
        }
    }
};