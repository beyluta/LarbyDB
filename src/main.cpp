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

int main(int argc, char** argv)
{
    bool backups_flag_set = false;
    bool allowBackup = false;
    bool port_set = false;
    char* portPtr;
    bool tables_set = false;
    int nTables;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (!backups_flag_set && strcmp(argv[i], "--backup") == 0)
            {
                allowBackup = true;
                backups_flag_set = true;
            }

            if (!backups_flag_set && strcmp(argv[i], "--nobackup") == 0)
            {
                allowBackup = false;
                backups_flag_set = true;
            }

            if (!port_set && strcmp(argv[i], "--port") == 0)
            {
                if (argv[i + 1] != nullptr)
                {
                    port_set = true;
                    portPtr = argv[i + 1];
                }
            }
            
            if (!tables_set && strcmp(argv[i], "--tables") == 0)
            {
                if (argv[i + 1] != nullptr)
                {
                    int n = atoi(argv[i + 1]);
                    if (n > 0)
                    {
                        nTables = n;
                        tables_set = true;
                    }
                }
            }
            
        }
    }

    Timer *backupTimer = new Timer();
    backupTimer->Subscribe(BackupHandler, 0);

    char *port;
    std::cout << "Port number: ";
    if (port_set)
    {
        strcpy(port, portPtr);
        std::cout << port << '\n';
    }
    else
    {
        std::cin >> port;
    }

    std::cout << "Number of tables in the database: ";
    if (tables_set)
    {
        std::cout << nTables << '\n';
    }
    else
    {
        std::cin >> nTables;
    }

    char allowBackupChar;
    if (backups_flag_set)
    {
        if(allowBackup)
        {
            std::cout << "Running with automatic backups.\n";
        }

        else
        {
            std::cout << "WARNING: Automatic backups will not be performed.\n";
        }
    }
    
    else
    {
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

    std::cout << "Database Tables Initialized(" << nTables << "), port: " << port << "." << std::endl;

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