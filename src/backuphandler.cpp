#include "main.h"

class BackupHandler
{
public:
    void BeginBackup(Controller &c)
    {
        for (int i = 0; i < c.GetSize(); i++)
        {
            c.ResolveStringCommand("FETCH /*/ FROM /" + std::to_string(i) + "/");
        }
    }
};