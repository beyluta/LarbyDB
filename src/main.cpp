#include "main.h"

Controller *controller;

const char *MessageReceived(const char *msg, const char* ip)
{
    std::cout << msg;
    std::cout << "ip address: " << ip << "\n";
    return controller->ResolveStringCommand(msg).c_str();
}

int BackupHandlerMessage(int var)
{
    BackupHandler handler(*controller, true);
    handler.BeginBackup();
    //handler.LoadBackup();
    return 0;
}

int main(int argc, char **argv)
{
    bool db_key_flag = false;
    bool db_safe = false;
    bool backups_flag_set = false;
    bool allowBackup = false;
    bool port_set = false;
    char *portPtr;
    bool tables_set = false;
    std::string nTablesStr;
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (!db_key_flag && strcmp(argv[i], "--safe") == 0)
            {
                db_safe = true;
                db_key_flag = true;
            }

            if (!db_key_flag && strcmp(argv[i], "--unsafe") == 0)
            {
                db_safe = false;
                db_key_flag = true;
            }

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
                    nTablesStr = argv[i + 1];
                    tables_set = true;
                }
            }
        }
    }

    Timer *backupTimer = new Timer();
    backupTimer->Subscribe(BackupHandlerMessage, 0);

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
        std::cout << nTablesStr << '\n';
    }
    else if (!tables_set)
    {
        std::cin >> nTablesStr;
    }
    int nTables = atoi(nTablesStr.c_str());
    while ( nTables < 2)
    {
        std::cout << "Number of tables in the database: ";
        std::cin >> nTablesStr;
        nTables = atoi(nTablesStr.c_str());
    }

    char allowBackupChar;
    if (backups_flag_set)
    {
        if (allowBackup)
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
            //fails the check for some reason??
            while (allowBackupChar != 'y' && allowBackupChar != 'n' || allowBackupChar != 'Y' && allowBackupChar != 'N')
            {
                std::cout << "Invalid input. Please enter 'y' or 'n': ";
                std::cin >> allowBackupChar;
            }
        }
    }

    if (db_key_flag)
    {
        /*
        if (db_safe)
        {
            std::cout << "Generating a key.\n";
        }

        else
        */
        if (!db_safe)
        {
            std::cout << "WARNING: All commands will be accessible without a key!\n";
        }
    }

    else
    {
        char dbSafeChar;
        std::cout << "Generate a key for accessing the database? (y/n): ";
        std::cin >> dbSafeChar;

        if (dbSafeChar == 'y' || dbSafeChar == 'Y')
        {
            db_safe = true;
            db_key_flag = true;
        }
        else if (dbSafeChar == 'n' || dbSafeChar == 'N')
        {
            std::cout << "WARNING: All commands will be accessible without a key!" << std::endl;
            db_safe = false;
            db_key_flag = true;
        }

        else
        {
            //same here. I have no idea...
            while (dbSafeChar != 'y' && dbSafeChar != 'n' || dbSafeChar != 'Y' && dbSafeChar != 'N')
            {
                std::cout << "Invalid input. Please enter 'y' or 'n': ";
                std::cin >> dbSafeChar;
            }
        }
    }

    controller = new Controller(nTables);

    /* Creating the socket and listening for connections */
    OnMessageReceived = &MessageReceived;
    Socket *socket = new Socket(port);

    std::cout << "Database Tables Initialized(" << nTables << "), port: " << port << "." << std::endl;
    if (db_safe)
    {
        std::cout << "Key: " << controller->GenerateKey() << "\n";
    }

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