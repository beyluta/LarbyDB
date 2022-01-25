#include "main.h"

Controller *controller;

/* Event handler which triggers when a socket message is received
   through an available TCP port. The message and IP address of the client
   will be exposed during the transaction. */
std::string MessageReceived(const char *msg, const char *ip)
{
    controller->tempIP = ip;
    return controller->GetResolvedResponse(msg);
}

/* Thread timer which triggers every couple of seconds to peform a backup */
int BackupHandlerMessage(int var)
{
    BackupHandler handler(controller, true);
    handler.BeginBackup();
    return 0;
}

/* Thread timer which triggers every second and is responsible for counting
   down the time_to_live variable of every TTL-Enabled piece of data. When the
   TTL reaches 0, it effectively expires and is removed from the database.*/
int TTLTimer(int arg)
{
    for (int i = 0; i < controller->packets.size(); i++)
    {
        if (controller->packets.at(i).time_to_live > 0)
        {
            controller->packets.at(i).time_to_live--;
        }
        else
        {
            int table = controller->packets.at(i).table;
            int hash = controller->packets.at(i).hash;
            controller->packets.erase(controller->packets.begin() + i);
            controller->hashtables[table].Remove(hash);
        }
    }
    return 0;
}

int main(int argc, char **argv)
{
    /*this part processes command line arguments.*/
    bool autoload = false;
    bool db_key_flag = false;
    bool db_safe = false;
    bool backups_flag_set = false;
    bool allowBackup = false;
    bool port_set = false;
    char *portPtr;
    bool tables_set = false;
    std::string nTablesStr;
    if (argc > 1) //TODO: This needs to be improved
    {
        for (int i = 1; i < argc; i++)
        {

            if (strcmp(argv[i], "--autoload") == 0)
            {
                autoload = true;
            }

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

    //TODO: there probably is a way to make the timers stack allocated. Look into that.
    Timer *backupTimer = new Timer();
    Timer *ttlTimer = new Timer();
    backupTimer->Subscribe(BackupHandlerMessage, 0);
    ttlTimer->Subscribe(TTLTimer, 0);

    /*This part is for configuring the database on launch.*/
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
    while (nTables < 2)
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
            while (allowBackupChar != 'y' && allowBackupChar != 'n' && allowBackupChar != 'Y' && allowBackupChar != 'N')
            {
                std::cout << "Invalid input. Please enter 'y' or 'n': ";
                std::cin >> allowBackupChar;
            }
        }
    }

    if (db_key_flag)
    {
        if (!db_safe)
        {
            std::cout << "WARNING: Running in unsafe mode. All commands will be accessible without a key!\n";
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
            std::cout << "WARNING: Running in unsafe mode. All commands will be accessible without a key!" << std::endl;
            db_safe = false;
            db_key_flag = true;
        }

        else
        {
            while (dbSafeChar != 'y' && dbSafeChar != 'n' && dbSafeChar != 'Y' && dbSafeChar != 'N')
            {
                std::cout << "Invalid input. Please enter 'y' or 'n': ";
                std::cin >> dbSafeChar;
            }
        }
    }

    //TODO: the controller needs to be global, same goes for the backuphandler.
    controller = new Controller(nTables);

    BackupHandler handler2(controller, true);
    if (handler2.CheckBackup() == true)
    {
        //std::cin.clear();
        char loadFromBackupChar;
        if (autoload == true)
        {
            loadFromBackupChar = {'y'};
        }
        else
        {
            std::cout << "Backup file found. Load from backup? (y/n): ";
            std::cin >> loadFromBackupChar;
        }

        while (loadFromBackupChar != 'Y' && loadFromBackupChar != 'N' && loadFromBackupChar != 'y' && loadFromBackupChar != 'n')
        {
            std::cout << "Invalid input. Please enter 'y' or 'n': ";
            std::cin >> loadFromBackupChar;
        }
        if (loadFromBackupChar == 'Y' || loadFromBackupChar == 'y')
        {
            handler2.LoadBackup();
            std::cout << "Loaded from backup.\n";
        }
        else
        {
            std::cout << "Backup will not be loaded.\n";
        }
    }

    OnMessageReceived = &MessageReceived;
    Socket *socket = new Socket(port);

    std::cout << "Database Tables Initialized(" << nTables << "), port: " << port << "." << std::endl;
    if (db_safe)
    {
        controller->protectedByKey = true;
        std::cout << "Key: " << controller->GenerateKey() << "\n";
    }

    if (allowBackup)
    {
        backupTimer->Start(1);
    }
    ttlTimer->Start(1);
    socket->Listen();

    delete socket;
    delete controller;
    delete backupTimer;
    delete ttlTimer;

    return 0;
}