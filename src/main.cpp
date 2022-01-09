#include "main.h"

Controller *controller;

const char *MessageReceived(const char *msg)
{
    return controller->ResolveStringCommand(msg).c_str();
}

int BackupHandler(int var)
{
    std::cout << "Peforming Backup..." << std::endl;
    return 0;
}

int main()
{
    Timer *backupTimer = new Timer();
    backupTimer->Subscribe(BackupHandler, 0);

    char *port;
    std::cout << "Port number: ";
    std::cin >> port;

    int nTables;
    std::cout << "Number of tables in the database: ";
    std::cin >> nTables;

    bool allowBackup = false;
    char allowBackupChar;
    std::cout << "Allow automatic backups? (y/n): ";
    std::cin >> allowBackupChar;

    if (allowBackupChar == 'y' || allowBackupChar == 'Y')
    {
        allowBackup = true;
    }
    else if (allowBackupChar == 'n' || allowBackupChar == 'N')
    {
        std::cout << "WARNING: Backups will not be performed." << std::endl;
    }
    else
    {
        while (allowBackupChar != 'y' && allowBackupChar != 'n' || allowBackupChar != 'Y' && allowBackupChar != 'N')
        {
            std::cout << "Invalid input. Please enter 'y' or 'n': ";
            std::cin >> allowBackupChar;
        }
    }

    while (std::cin.fail())
    {
        std::cin.clear();
        std::string emptyStr;
        std::getline(std::cin, emptyStr);
        std::cout << "Number of tables in the database: ";
        std::cin >> nTables;
    }
    controller = new Controller(nTables);

    /* Creating the socket and listening for connections */
    OnMessageReceived = &MessageReceived;
    Socket *socket = new Socket(port);

    std::cout << "Database Tables Initialized(" << nTables << "), port: " << port << "\n";

    if (allowBackup)
    {
        backupTimer->Start(1);
    }

    socket->Listen();
    delete socket;
    delete controller;
    delete backupTimer;

    return 0;
}