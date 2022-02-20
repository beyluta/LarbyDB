#include "main.h"
#define DEFAULT_NUM_TABLES 2
#define DEFAULT_PORT "8080"
#define DEFAULT_MANUAL_CONFIG false
#define DEFAULT_GENERATE_KEY true
#define DEFAULT_ALLOW_BACKUP true
#define DEFAULT_BACKUP_INTERVAL "30m"

Controller *controller;

/* Event handler which triggers when a socket message is received
   through an available TCP port. The message and IP address of the client
   will be exposed during the transaction. */
std::string MessageReceived(const char *msg, const char *ip)
{
    if (strlen(msg) > 0)
    {
        controller->tempIP = ip;
        return controller->GetResolvedResponse(msg);
    }
    return GetHttpStatusCode(404);
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

struct Parameters
{
    string port;
    int num_tables;
    bool generate_key;
    bool allow_backup;
    bool manual_config;
    int backup_interval;
};

string BoolToStr(bool b)
{
    return b ? "true" : "false";
}

vector<string> ProcessConfig()
{
    Parameters parameters;
    File configFile;
    ifstream file("config.conf");
    string line;
    vector<string> args;

    if (!configFile.FileExists("config.conf"))
    {
        configFile.CreateFile("config.conf");
    }

    if (configFile.FileExists("config.conf"))
    {
        while (getline(file, line))
        {
            int comment = line.find("#");
            if (comment == 0)
                continue;
            if (comment != string::npos)
            {
                line = line.substr(0, comment);
            }
            else
            {
                line = "--" + line;
            }
            stringstream ss(line);
            string arg;
            while (getline(ss, arg, ' '))
            {
                if (arg == "")
                    continue;
                args.push_back(arg);            
            }
        }

        if (args.size() < 1) //if empty, fill in with default values.
        {
            configFile.OverwriteFile("config.conf",
            "#LarbyDB config\nmanual_config "+BoolToStr(DEFAULT_MANUAL_CONFIG)+
            "\nport "+string(DEFAULT_PORT)+
            "\nnum_tables "+to_string(DEFAULT_NUM_TABLES)+
            "\nallow_backup "+BoolToStr(DEFAULT_ALLOW_BACKUP)+
            "\nbackup_interval "+string(DEFAULT_BACKUP_INTERVAL)+
            "\ngenerate_key "+BoolToStr(DEFAULT_GENERATE_KEY));
            return ProcessConfig();
        }
    }
    return args;
}

void SetParameters(Parameters& params, vector<string>& str)
{
    for (int i = 0; i < str.size() - 1; i++)
    {
        // std::cout << "[" << i << "]#" << str[i] << "#\n";
        // std::cout << "[" << i + 1 << "]##" << str[i + 1] << "##\n";
        if (str[i].find("--port") != string::npos)
        {
            params.port = str[i + 1];
        }
        else if (str[i].find("--num_tables") != string::npos)
        {
            int num = atoi(str[i + 1].c_str());
            if (num > 1)
                params.num_tables = num;
            else
                params.num_tables = DEFAULT_NUM_TABLES;
        }
        else if (str[i].find("--backup_interval") != string::npos)
        {
            int interval = atoi(str[i + 1].c_str());
            if (interval < 1)
            {
                params.backup_interval = atoi(DEFAULT_BACKUP_INTERVAL);
            }
            else
            {
                if (str[i + 1].find("h") == str[i + 1].size() - 1)
                    params.backup_interval = interval * 3600;
                else if (str[i + 1].find("m") == str[i + 1].size() - 1)
                    params.backup_interval = interval * 60;
                else
                    params.backup_interval = interval;
            }
        }

        if (str[i].find("--generate_key") != string::npos)
        {
            if (str[i + 1] == "true")
                params.generate_key = true;
            else if (str[i + 1] == "false")
                params.generate_key = false;
            else
                params.generate_key = DEFAULT_GENERATE_KEY;
        }
        if (str[i].find("--allow_backup") != string::npos)
        {
            if (str[i + 1] == "true")
                params.allow_backup = true;
            else if (str[i + 1] == "false")
                params.allow_backup = false;
            else
                params.allow_backup = DEFAULT_ALLOW_BACKUP;
        }
        if (str[i].find("--manual_config") != string::npos)
        {
            if (str[i + 1] == "true")
                params.manual_config = true;
            else if (str[i + 1] == "false")
                params.manual_config = false;
            else
                params.manual_config = DEFAULT_MANUAL_CONFIG;
        }

    }
}

int main(int argc, char **argv)
{
    vector<string> configArguments = ProcessConfig();
    Parameters dbParameters;
    SetParameters(dbParameters, configArguments);
    if (argc > 1)
    {
        vector<string> launchArguments(argv + 1, argv + argc);
        SetParameters(dbParameters, launchArguments);
    }

    // epic-looking ascii art
    cout
    << " ###################################################\n"
    << "##...._..............._.............____..____.....##\n"
    << "##...|.|....__._._.__|.|__.._..._..|.._.\\|.__.)....##\n"
    << "##...|.|.../._`.|.'__|.'_.\\|.|.|.|.|.|.|.|.._.\\....##\n"
    << "##...|.|__|.(_|.|.|..|.|_).|.|_|.|.|.|_|.|.|_).|...##\n"
    << "##...|_____\\__,_|_|..|_.__/.\\__,.|.|____/|____/....##\n"
    << "##..........................|___/..................##\n"
    << " ###################################################\n"
    << "\n";

    // prompt the user if manual_config is set to true:
    if (dbParameters.manual_config)
    {
        std::cout << "TODO: MANUAL CONFIG\n";
    }
    else
    {
    cout
    << "port: " << dbParameters.port << '\n'
    << "tables: " << dbParameters.num_tables << '\n'
    << "allow_backup: " << BoolToStr(dbParameters.allow_backup) << '\n'
    << "backup_interval: "
    << (dbParameters.allow_backup ? to_string(dbParameters.backup_interval)+" seconds" : "unset")
    << '\n'
    << "generate_key: " << BoolToStr(dbParameters.generate_key) << '\n';
    }

    signal(SIGPIPE, SIG_IGN);
    /*this part processes command line arguments.*/
    /*
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
    */
    //TODO: there probably is a way to make the timers stack allocated. Look into that.
    Timer *backupTimer = new Timer();
    Timer *ttlTimer = new Timer();
    backupTimer->Subscribe(BackupHandlerMessage, 0);
    ttlTimer->Subscribe(TTLTimer, 0);

    /*This part is for configuring the database on launch.*/
    // char *port;
    // std::cout << "Port number: ";
    // if (port_set)
    // {
    //     strcpy(port, portPtr);
    //     std::cout << port << '\n';
    // }
    // else
    // {
    //     std::cin >> port;
    // }

    // std::cout << "Number of tables in the database: ";
    // if (tables_set)
    // {
    //     std::cout << nTablesStr << '\n';
    // }
    // else if (!tables_set)
    // {
    //     std::cin >> nTablesStr;
    // }
    // int nTables = atoi(nTablesStr.c_str());
    // while (nTables < 2)
    // {
    //     std::cout << "Number of tables in the database: ";
    //     std::cin >> nTablesStr;
    //     nTables = atoi(nTablesStr.c_str());
    // }

    // char allowBackupChar;
    // if (backups_flag_set)
    // {
    //     if (allowBackup)
    //     {
    //         std::cout << "Running with automatic backups.\n";
    //     }

    //     else
    //     {
    //         std::cout << "WARNING: Automatic backups will not be performed.\n";
    //     }
    // }

    // else
    // {
    //     std::cout << "Allow automatic backups? (y/n): ";
    //     std::cin >> allowBackupChar;

    //     if (allowBackupChar == 'y' || allowBackupChar == 'Y')
    //     {
    //         allowBackup = true;
    //     }
    //     else if (allowBackupChar == 'n' || allowBackupChar == 'N')
    //     {
    //         std::cout << "WARNING: Backups will not be performed." << std::endl;
    //     }
    //     else
    //     {
    //         while (allowBackupChar != 'y' && allowBackupChar != 'n' && allowBackupChar != 'Y' && allowBackupChar != 'N')
    //         {
    //             std::cout << "Invalid input. Please enter 'y' or 'n': ";
    //             std::cin >> allowBackupChar;
    //         }
    //     }
    // }

    // if (db_key_flag)
    // {
    //     if (!db_safe)
    //     {
    //         std::cout << "WARNING: Running in unsafe mode. All commands will be accessible without a key!\n";
    //     }
    // }

    // else
    // {
    //     char dbSafeChar;
    //     std::cout << "Generate a key for accessing the database? (y/n): ";
    //     std::cin >> dbSafeChar;

    //     if (dbSafeChar == 'y' || dbSafeChar == 'Y')
    //     {
    //         db_safe = true;
    //         db_key_flag = true;
    //     }
    //     else if (dbSafeChar == 'n' || dbSafeChar == 'N')
    //     {
    //         std::cout << "WARNING: Running in unsafe mode. All commands will be accessible without a key!" << std::endl;
    //         db_safe = false;
    //         db_key_flag = true;
    //     }

    //     else
    //     {
    //         while (dbSafeChar != 'y' && dbSafeChar != 'n' && dbSafeChar != 'Y' && dbSafeChar != 'N')
    //         {
    //             std::cout << "Invalid input. Please enter 'y' or 'n': ";
    //             std::cin >> dbSafeChar;
    //         }
    //     }
    // }

    //TODO: the controller needs to be global, same goes for the backuphandler.
    controller = new Controller(dbParameters.num_tables);

    BackupHandler handler2(controller, true);
    // if (handler2.CheckBackup() == true)
    // {
    //     //std::cin.clear();
    //     char loadFromBackupChar;
    //     if (autoload == true)
    //     {
    //         loadFromBackupChar = {'y'};
    //     }
    //     else
    //     {
    //         std::cout << "Backup file found. Load from backup? (y/n): ";
    //         std::cin >> loadFromBackupChar;
    //     }

    //     while (loadFromBackupChar != 'Y' && loadFromBackupChar != 'N' && loadFromBackupChar != 'y' && loadFromBackupChar != 'n')
    //     {
    //         std::cout << "Invalid input. Please enter 'y' or 'n': ";
    //         std::cin >> loadFromBackupChar;
    //     }
    //     if (loadFromBackupChar == 'Y' || loadFromBackupChar == 'y')
    //     {
            handler2.LoadBackup();
    //         std::cout << "Loaded from backup.\n";
    //     }
    //     else
    //     {
    //         std::cout << "Backup will not be loaded.\n";
    //     }
    // }
    const char* port = dbParameters.port.c_str();
    OnMessageReceived = &MessageReceived;
    Socket *socket = new Socket(port);

    if (dbParameters.generate_key)
    {
        controller->protectedByKey = true;
        std::cout << "Key: " << controller->GenerateKey() << "\n";
    }
    else
    {
        std::cout << "WARNING: Running in unsafe mode. All commands will be accessible without a key!\n";
    }
    ttlTimer->Start(1);
    std::cout << "Database Initialized. " << std::endl;
    socket->Listen();

    delete socket;
    delete controller;
    delete backupTimer;
    delete ttlTimer;

    return 0;
}