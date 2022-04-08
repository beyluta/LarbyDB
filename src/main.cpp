#include "main.h"

using namespace std;

Controller controller;
Socket serverSocket;
Timer backupTimer;
Timer ttlTimer;

std::string MessageReceived(const char *msg, const char *ip)
{
    controller.tempIP = ip;
    return strlen(msg) > 0 ? controller.GetResolvedResponse(msg) : GetHttpStatusCode(404);
}

int BackupHandlerMessage(int var)
{
    BackupHandler handler(&controller, true);
    handler.BeginBackup();
    return 0;
}

int TTLTimer(int arg)
{
    for (int i = 0; i < controller.packets.size(); i++)
    {
        if (controller.packets.at(i).time_to_live > 0)
        {
            controller.packets.at(i).time_to_live--;
            continue;
        }

        int table = controller.packets.at(i).table;
        int hash = controller.packets.at(i).hash;
        controller.packets.erase(controller.packets.begin() + i);
        controller.hashtables[table].Remove(hash);
    }
    return 0;
}

void OnInterrupt(int sigInt)
{
    cout << "\nStopped." << endl;
    exit(sigInt);
}

int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, OnInterrupt);
    Parameters dbParameters;
    {
        vector<string> configArguments = ProcessConfig();
        int configSettings = SetParameters(dbParameters, configArguments);
        if (NUM_SETTINGS - configSettings > 0)
            cout << "WARNING: config settings unset (" << NUM_SETTINGS - configSettings << " out of " << NUM_SETTINGS << "), using fallback values.\n";

        if (argc > 1)
        {
            vector<string> launchArguments(argv + 1, argv + argc);
            SetParameters(dbParameters, launchArguments);
        }
    }

    if (dbParameters.manual_config)
    {
        auto PromptBool = [&](const char *msg, bool &setting, bool defaultVal)
        {
            int result = PromptYN(msg);
            if (result >= 0 && result <= 1)
            {
                setting = result;
            }
            else
            {
                setting = DEFAULT_ALLOW_BACKUP;
                cout << boolalpha << "Invalid input, using the default value (" << setting << ").\n";
            }
        };

        cout << "LarbyDB manual configuration mode.\n"
             << "To Disable manual configuration, run with '--manual_config false'\n"
             << "or set 'manual_config' to 'false' in config.conf\n\n";

        cout << "enter port: ";
        cin >> dbParameters.port;

        cout << "enter the number of tables for the database (minimum is 2): ";
        string numTablesInput;
        cin >> numTablesInput;
        int numTables = atoi(numTablesInput.c_str());
        if (numTables > 1)
        {
            dbParameters.num_tables = numTables;
        }
        else
        {
            dbParameters.num_tables = DEFAULT_NUM_TABLES;
            cout << "Invalid input, using the default value (" << dbParameters.num_tables << ").\n";
        }

        PromptBool("allow automatic backup? (y/n): ", dbParameters.allow_backup, DEFAULT_ALLOW_BACKUP);

        if (dbParameters.allow_backup)
        {
            cout << "set the backup interval: ";
            string inputBackupStr;
            cin >> inputBackupStr;
            ParseBackupInterval(dbParameters, inputBackupStr);
        }
        else
        {
            string dflt = DEFAULT_BACKUP_INTERVAL;
            ParseBackupInterval(dbParameters, dflt);
        }
        PromptBool("generate authentication key? (y/n): ", dbParameters.generate_key, DEFAULT_GENERATE_KEY);
    }

    backupTimer.Subscribe(BackupHandlerMessage, 0);
    ttlTimer.Subscribe(TTLTimer, 0);

    controller.SetSize(dbParameters.num_tables);
    {
        BackupHandler handler2(&controller, true);
        if (handler2.CheckBackup())
        {
            if (dbParameters.load_backup == "true")
            {
                handler2.LoadBackup();
            }
            else if (dbParameters.load_backup == "ask")
            {
                int shouldLoad = PromptYN("Backup file found. Load from backup? (y/n): ");
                while (shouldLoad < 0)
                {
                    shouldLoad = PromptYN("Invalid input, enter 'y' or 'n': ");
                }
                if (shouldLoad >= 1)
                {
                    handler2.LoadBackup();
                    cout << "Loaded from backup.\n";
                }
                else
                {
                    cout << "loading canceled.\n";
                }
            }
            else
                cout << "loading from backup is disabled.\n";
        }
    }
    const char *port = dbParameters.port.c_str();
    OnMessageReceived = &MessageReceived;
    serverSocket.SetPort(port);

    if (dbParameters.allow_backup)
    {
        backupTimer.Start(dbParameters.backup_interval);
    }
    ttlTimer.Start(1);

    cout
        << "\n\n"
        << " ###################################################\n"
        << "##...._..............._.............____..____.....##\n"
        << "##...| |....__._._.__| |__.._..._..|  _ \\| __ )....##\n"
        << "##...| |.../ _` | '__| '_ \\| |.| |.| |.| |  _ \\....##\n"
        << "##...| |__| (_| | |..| |_) | |_| |.| |_| | |_) |...##\n"
        << "##...|_____\\__,_|_|..|_.__/.\\__, |.|____/|____/....##\n"
        << "##..........................|___/..................##\n"
        << " ###################################################\n"
        << "\n\n";

    cout
        << "port: " << dbParameters.port << '\n'
        << "tables: " << dbParameters.num_tables << '\n'
        << "allow_backup: " << BoolToStr(dbParameters.allow_backup) << '\n'
        << "backup_interval: "
        << (dbParameters.allow_backup ? to_string(dbParameters.backup_interval) + " seconds" : "unset")
        << '\n'
        << "generate_key: " << BoolToStr(dbParameters.generate_key) << '\n';

    if (dbParameters.generate_key)
    {
        controller.protectedByKey = true;
        controller.GenerateKey();
        std::cout << "Key: " << controller.hashtables[0].Get(DB_KEY_POSITION) << "\n";
    }
    else
    {
        std::cout << "WARNING: Running in unsafe mode. All commands will be accessible without a key!\n";
    }
    cout << "Database Initialized. " << std::endl;

    serverSocket.Listen();
    return 0;
}