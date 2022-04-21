#include <iostream>
#include <cstring>
#include "socket.h"
#include "hashtable.h"
#include "controller.h"
#include "timer.h"
#include "backuphandler.h"
#include "signal.h"
#include "config.h"
#include "log4c.h"

Controller controller;
Socket serverSocket;
Timer backupTimer;
Timer ttlTimer;
Log4c log4c;

std::string MessageReceived(const char *msg, const char *ip)
{
    controller.tempIP = ip;
    std::string response = strlen(msg) > 0 ? controller.GetResolvedResponse(msg) : "Not Found";
    return response + "\r\n\0";
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
    log4c.LogActivity("Database Interrupted", Log4c::IOSystem::BOTH, Log4c::Color::GREEN);
    exit(sigInt);
}

int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGINT, OnInterrupt);
    Parameters dbParameters;
    {
        std::vector<std::string> configArguments = ProcessConfig();
        int configSettings = SetParameters(dbParameters, configArguments);
        if (DEFAULT_NUM_SETTINGS - configSettings > 0)
        {
            cout << "WARNING: config settings unset (" << DEFAULT_NUM_SETTINGS - configSettings << " out of " << DEFAULT_NUM_SETTINGS << "), using fallback values.\n";
        }

        if (argc > 1)
        {
            std::vector<std::string> launchArguments(argv + 1, argv + argc);
            SetParameters(dbParameters, launchArguments);
        }
    }

    if (dbParameters.manual_config)
    {
        auto PromptBool = [&](const char *msg, bool &setting, bool defaultVal)
        {
            UserInputState answer = PromptYN(msg);

            if (answer == YES || answer == NO)
            {
                setting = answer;
            }
            else
            {
                setting = DEFAULT_ALLOW_BACKUP;
                std::cout << boolalpha << "Invalid input, using the default value (" << setting << ").\n";
            }
        };

        std::cout << "LarbyDB manual configuration mode.\n"
                  << "To Disable manual configuration, run with '--manual_config false'\n"
                  << "or set 'manual_config' to 'false' in config.conf\n\n";

        std::cout << "enter port: ";
        std::cin >> dbParameters.port;

        cout << "enter the number of tables for the database (minimum is 2): ";
        std::string numTablesInput;
        std::cin >> numTablesInput;
        int numTables = atoi(numTablesInput.c_str());

        if (numTables > 1)
        {
            dbParameters.num_tables = numTables;
        }
        else
        {
            dbParameters.num_tables = DEFAULT_NUM_TABLES;
            std::cout << "Invalid input, using the default value (" << dbParameters.num_tables << ").\n";
        }

        PromptBool("allow automatic backup? (y/n): ", dbParameters.allow_backup, DEFAULT_ALLOW_BACKUP);

        if (dbParameters.allow_backup)
        {
            std::cout << "set the backup interval: ";
            std::string inputBackupStr;
            std::cin >> inputBackupStr;
            ParseBackupInterval(dbParameters, inputBackupStr);
        }
        else
        {
            std::string dflt = DEFAULT_BACKUP_INTERVAL;
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
                UserInputState answer = PromptYN("Backup file found. Load from backup? (y/n): ");

                while (answer == INVALID)
                {
                    answer = PromptYN("Invalid input, enter 'y' or 'n': ");
                }

                if (answer == YES)
                {
                    handler2.LoadBackup();
                    std::cout << "Loaded from backup.\n";
                }
                else
                {
                    std::cout << "loading canceled.\n";
                }
            }
            else
            {
                std::cout << "loading from backup is disabled.\n";
            }
        }
    }

    const char *port = dbParameters.port.c_str();
    serverSocket.OnMessageReceived = &MessageReceived;
    serverSocket.SetPort(port);

    if (dbParameters.allow_backup)
    {
        backupTimer.Start(dbParameters.backup_interval);
    }

    ttlTimer.Start(1);

    std::cout
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

    std::cout
        << "Version: " << SEMANTIC_VERSION << '\n'
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

    log4c.LogActivity("Database Initialized", Log4c::IOSystem::BOTH, Log4c::Color::GREEN);

    serverSocket.Listen();
    return 0;
}