#include <iostream>
#include <cstring>
#include <signal.h>
#include <string.h>
#include "socket.h"
#include "hashtable.h"
#include "controller.h"
#include "timer.h"
#include "backuphandler.h"
#include "config.h"
#include "http.h"
#include "logsys.h"

Controller controller;
Socket serverSocket;
Timer timer;
Parameters dbParameters;

std::string MessageReceived(const char *msg, const char *ip)
{
    Logsys::LogActivity("Processing request from " + std::string(ip) + " of char size " + std::to_string(strlen(msg)) + " out of a maximum of " + std::to_string(MAX_BUFFER_SIZE), Logsys::IOSystem::BOTH, Logsys::Color::GREEN);
    controller.tempIP = ip;

    if (dbParameters.enable_http)
    {        
        if (strlen(msg) >= MAX_BUFFER_SIZE)
        {
            return GetHTTPResponse(HTTPResponseCode::CONTENT_TOO_LARGE, "application/json", "{\"status\": 413, \"message\": \"Payload is too large to handle\"}");
        }

        HTTP httpResponse = GetHTTP(msg);
        std::string contentType = httpResponse.properties.Get("content-type");
        std::string bearerToken = httpResponse.properties.Get("authorization");
        bearerToken = bearerToken.length() > 0 ? bearerToken.substr(7, bearerToken.length() - 7) : "";
        int table = atoi(httpResponse.parameters.Get("table").c_str());
        std::string key = httpResponse.parameters.Get("key");

        if (bearerToken.length() <= 0 || controller.Auth(bearerToken) > 0)
        {
            return GetHTTPResponse(HTTPResponseCode::UNAUTHORIZED, "application/json", "{\"status\": 401, \"message\": \"Unauthorized\"}");
        }

        if (table <= 0)
        {
            return GetHTTPResponse(HTTPResponseCode::FORBIDDEN, "application/json", "{\"status\": 403, \"message\": \"Forbidden. Table out of bounds.\"}");
        }

        if (httpResponse.method == "GET")
        {
            if (key == "ALL")
            {
                return GetHTTPResponse(HTTPResponseCode::OK, contentType, controller.GetAll(table));
            }

            std::string value = controller.Get(key, table);

            if (value.length() <= 0)
            {
                return GetHTTPResponse(HTTPResponseCode::NOT_FOUND, "application/json", "{\"status\": 404, \"message\": \"Not Found\"}");
            }

            if (contentType.length() > 0)
            {
                return GetHTTPResponse(HTTPResponseCode::OK, contentType, value);
            }

            return GetHTTPResponse(HTTPResponseCode::OK, "application/json", value);
        }

        if (httpResponse.method == "POST")
        {
            if (controller.Set(key, httpResponse.body, table) > 0)
            {
                return GetHTTPResponse(HTTPResponseCode::INTERNAL_SERVER_ERROR, "application/json", "{\"status\": 500, \"message\": \"Internal server error\"}");
            }

            return GetHTTPResponse(HTTPResponseCode::OK, "application/json", "{\"status\": 200, \"message\": \"Ok\"}");
        }

        if (httpResponse.method == "DELETE")
        {
            if (controller.Delete(key, table) > 0)
            {
                return GetHTTPResponse(HTTPResponseCode::INTERNAL_SERVER_ERROR, "application/json", "{\"status\": 500, \"message\": \"Internal server error\"}");
            }

            return GetHTTPResponse(HTTPResponseCode::OK, "application/json", "{\"status\": 200, \"message\": \"Ok\"}");
        }
    }

    if (strlen(msg) >= MAX_BUFFER_SIZE)
    {
        return "Payload is too large to handle\r\n\0";
    }

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
    cout << "\n";
    Logsys::LogActivity("\nInterrupt signal received.", Logsys::IOSystem::BOTH, Logsys::Color::GREEN);
    exit(sigInt);
}

int main(int argc, char **argv)
{
    #if _WIN32
    signal(13, SIG_IGN);
    #elif __unix__ || __APPLE__
    signal(SIGPIPE, SIG_IGN);
    #endif
    signal(SIGINT, OnInterrupt);
    {
        std::vector<std::string> configArguments = ProcessConfig();
        int configSettings = SetParameters(dbParameters, configArguments);
        if (DEFAULT_NUM_SETTINGS - configSettings > 0)
        {
            Logsys::LogActivity("WARNING: config settings unset (" + std::to_string(DEFAULT_NUM_SETTINGS - configSettings) + " out of " + std::to_string(DEFAULT_NUM_SETTINGS) + "), using fallback values", Logsys::IOSystem::TERMINAL, Logsys::Color::YELLOW, false);
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
                std::cout << std::boolalpha << "Invalid input, using the default value (" << setting << ").\n";
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
            Logsys::LogActivity("Invalid input, using the default value (" + std::to_string(dbParameters.num_tables) + ")", Logsys::IOSystem::TERMINAL, Logsys::Color::RED, false);
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

        PromptBool("enable HTTP? (y/n): ", dbParameters.enable_http, DEFAULT_ENABLE_HTTP);
    }

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
                    Logsys::LogActivity("Loaded from backup", Logsys::IOSystem::BOTH, Logsys::Color::YELLOW, false);
                }
                else
                {
                    Logsys::LogActivity("Loading canceled", Logsys::IOSystem::BOTH, Logsys::Color::YELLOW, false);
                }
            }
            else
            {
                Logsys::LogActivity("Loading from backup is disabled", Logsys::IOSystem::BOTH, Logsys::Color::YELLOW, false);
            }
        }
    }

    const char *port = dbParameters.port.c_str();
    serverSocket.OnMessageReceived = &MessageReceived;
    serverSocket.PortSet(port);


    if (dbParameters.allow_backup)
    {
        timer.Subscribe(BackupHandlerMessage, dbParameters.backup_interval);
    }

    timer.Subscribe(TTLTimer, 1);
    timer.Start();

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
        << "Transfer/Transmission protocol: " << (dbParameters.enable_http ? "HTTP" : "TCP") << '\n'
        << "tables: " << dbParameters.num_tables << '\n'
        << "allow_backup: " << BoolToStr(dbParameters.allow_backup) << '\n'
        << "backup_interval: "
        << (dbParameters.allow_backup ? std::to_string(dbParameters.backup_interval) + " seconds" : "unset")
        << '\n'
        << "generate_key: " << BoolToStr(dbParameters.generate_key) << '\n';

    if (dbParameters.generate_key)
    {
        controller.protectedByKey = true;
        controller.GenerateKey();
        Logsys::LogActivity("Key: " + controller.hashtables[0].Get(DB_KEY_POSITION), Logsys::IOSystem::TERMINAL, Logsys::Color::WHITE, false);
    }
    else
    {
        Logsys::LogActivity("WARNING: Running in unsafe mode. All commands will be accessible without a key!", Logsys::IOSystem::TERMINAL, Logsys::Color::YELLOW);
    }

    Logsys::LogActivity("Database Initialized", Logsys::IOSystem::BOTH, Logsys::Color::GREEN);

    serverSocket.Listen();
    return 0;
}