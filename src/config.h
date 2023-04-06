#include "file.h"
#include <sstream>
#include <fstream>

#define DEFAULT_PORT "8080"
#define DEFAULT_MANUAL_CONFIG false
#define DEFAULT_GENERATE_KEY true
#define DEFAULT_ALLOW_BACKUP true
#define DEFAULT_ENABLE_HTTP false
#define DEFAULT_NUM_TABLES 2
#define DEFAULT_BACKUP_INTERVAL "30m"
#define DEFAULT_LOAD_BACKUP "ask"
#define DEFAULT_NUM_SETTINGS 8
#define SEMANTIC_VERSION "v1.0.1"

#define OPTION_MANUAL_CONFIG "manual-config"
#define OPTION_PORT "port"
#define OPTION_NUM_TABLES "num-tables"
#define OPTION_ALLOW_BACKUP "allow-backup"
#define OPTION_ENABLE_HTTP "enable-http"
#define OPTION_BACKUP_INTERVAL "backup-interval"
#define OPTION_GENERATE_KEY "generate-key"
#define OPTION_LOAD_BACKUP "load-backup"


struct Parameters
{
    string port;
    int num_tables;
    bool generate_key;
    bool allow_backup;
    bool manual_config;
    int backup_interval;
    string load_backup;
    bool enable_http;
};

enum UserInputState
{
    YES = 1,
    NO = 0,
    INVALID = -1,
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

            if (line == "" || comment == 0)
            {
                continue;
            }

            line = comment != string::npos ? line.substr(0, comment) : "--" + line;

            stringstream ss(line);
            string arg;

            while (getline(ss, arg, ' '))
            {
                if (arg == "")
                {
                    continue;
                }

                args.push_back(arg);
            }
        }

        if (args.size() < 1)
        {
            configFile.OverwriteFile("config.conf",
                                     "#LarbyDB config\n# If set to true, will ask the user to input parameters on launch:\n"+
                                         string(OPTION_MANUAL_CONFIG)+" "+BoolToStr(DEFAULT_MANUAL_CONFIG)+
                                         "\n\n# sets the port:\n"+
                                         OPTION_PORT+" "+DEFAULT_PORT+
                                         "\n\n# How many hashtables the db has (minimum 2):\n"+
                                         OPTION_NUM_TABLES+" " +to_string(DEFAULT_NUM_TABLES)+
                                         "\n\n# Should the db perform automatic backups:\n# true | false\n"+
                                         OPTION_ALLOW_BACKUP+" "+BoolToStr(DEFAULT_ALLOW_BACKUP)+
                                         "\n\n# Sets the time interval between performing backups:\n# h - hours\n# m - minutes\n# anything else counts as seconds.\n# Only positive integer values are allowed.\n"+
                                         OPTION_BACKUP_INTERVAL+" "+DEFAULT_BACKUP_INTERVAL+
                                         "\n\n# Generate the authentication key:\n"+
                                         OPTION_GENERATE_KEY" "+BoolToStr(DEFAULT_GENERATE_KEY)+
                                        "\n\n# Enable Hypertext Transfer Protocol:\n"+
                                         OPTION_ENABLE_HTTP+" "+BoolToStr(DEFAULT_ENABLE_HTTP)+
                                         "\n\n# Should the db load from a backup file:\n# true  - load from backups\n# false - do not load from backups\n# ask   - ask the user whether to load from the backup file\n"+
                                         OPTION_LOAD_BACKUP+" "+DEFAULT_LOAD_BACKUP);
            return ProcessConfig();
        }
    }
    return args;
}

void ParseBackupInterval(Parameters &params, string &intervalStr)
{
    int interval = atoi(intervalStr.c_str()) > 0 ? atoi(intervalStr.c_str()) : atoi(string(DEFAULT_BACKUP_INTERVAL).c_str());

    if (intervalStr.find("h") == intervalStr.size() - 1)
    {
        params.backup_interval = interval * 3600;
    }
    else if (intervalStr.find("m") == intervalStr.size() - 1)
    {
        params.backup_interval = interval * 60;
    }
    else
    {
        params.backup_interval = interval;
    }
}

int SetParameters(Parameters &params, vector<string> &str)
{
    bool settings_set[DEFAULT_NUM_SETTINGS]{false};
    enum
    {
        manual_config,
        port,
        num_tables,
        allow_backup,
        backup_interval,
        generate_key,
        load_backup,
        enable_http
    };
    int settingsNum = 0;

    for (int i = 0; i < str.size() - 1; i++)
    {
        if (!settings_set[manual_config] && str[i].find(OPTION_MANUAL_CONFIG) != string::npos)
        {
            settings_set[manual_config] = true;
            settingsNum++;

            if (str[i + 1] == "true")
            {
                params.manual_config = true;
            }
            else if (str[i + 1] == "false")
            {
                params.manual_config = false;
            }
            else
            {
                settingsNum--;
                params.manual_config = DEFAULT_MANUAL_CONFIG;
                cout << boolalpha << OPTION_MANUAL_CONFIG << ": unset, using default (" << params.manual_config << ")\n";
            }
            continue;
        }

        if (!settings_set[port] && str[i].find(OPTION_PORT) != string::npos)
        {
            settings_set[port] = true;

            if (atoi(str[i + 1].c_str()) > 0)
            {
                settingsNum++;
                params.port = str[i + 1];
            }
            else
            {
                params.port = std::move(string(DEFAULT_PORT));
                cout << OPTION_PORT << ": unset, using default (" << params.port << ")\n";
            }
            continue;
        }

        if (!settings_set[num_tables] && str[i].find(OPTION_NUM_TABLES) != string::npos)
        {
            settings_set[num_tables] = true;
            int num = atoi(str[i + 1].c_str());

            if (num > 1)
            {
                settingsNum++;
                params.num_tables = num;
            }
            else
            {
                params.num_tables = DEFAULT_NUM_TABLES;
                cout << OPTION_NUM_TABLES << ": unset, using default (" << params.num_tables << ")\n";
            }
            continue;
        }

        if (!settings_set[backup_interval] && str[i].find(OPTION_BACKUP_INTERVAL) != string::npos)
        {
            settings_set[backup_interval] = true;
            ParseBackupInterval(params, str[i + 1]);
            settingsNum++;
            continue;
        }

        if (!settings_set[generate_key] && str[i].find(OPTION_GENERATE_KEY) != string::npos)
        {
            settings_set[generate_key] = true;
            settingsNum++;

            if (str[i + 1] == "true")
            {
                params.generate_key = true;
            }
            else if (str[i + 1] == "false")
            {
                params.generate_key = false;
            }
            else
            {
                settingsNum--;
                params.generate_key = DEFAULT_GENERATE_KEY;
                cout << boolalpha << OPTION_GENERATE_KEY << ": unset, using default (" << params.generate_key << ")\n";
            }
            continue;
        }

        if (!settings_set[allow_backup] && str[i].find(OPTION_ALLOW_BACKUP) != string::npos)
        {
            settings_set[allow_backup] = true;
            settingsNum++;

            if (str[i + 1] == "true")
            {
                params.allow_backup = true;
            }
            else if (str[i + 1] == "false")
            {
                params.allow_backup = false;
            }
            else
            {
                settingsNum--;
                params.allow_backup = DEFAULT_ALLOW_BACKUP;
                cout << boolalpha << OPTION_ALLOW_BACKUP << ": unset, using default (" << params.allow_backup << ")\n";
            }
            continue;
        }

        if (!settings_set[load_backup] && str[i].find(OPTION_LOAD_BACKUP) != string::npos)
        {
            settings_set[load_backup] = true;

            if (str[i + 1] == "true" || str[i + 1] == "false" || str[i + 1] == "ask")
            {
                params.load_backup = str[i + 1];
                settingsNum++;
            }
            else
            {
                params.load_backup = DEFAULT_LOAD_BACKUP;
                cout << OPTION_LOAD_BACKUP << ": unset, using default (" << params.load_backup << ")\n";
            }
            continue;
        }

        if (!settings_set[enable_http] && str[i].find(OPTION_ENABLE_HTTP) != string::npos)
        {
            settings_set[enable_http] = true;
            settingsNum++;

            if (str[i + 1] == "true")
            {
                params.enable_http = true;
            }
            else if (str[i + 1] == "false")
            {
                params.enable_http = false;
            }
            else
            {
                settingsNum--;
                params.enable_http = DEFAULT_ENABLE_HTTP;
                cout << boolalpha << OPTION_ENABLE_HTTP << ": unset, using default (" << params.enable_http << ")\n";
            }
            continue;
        }
    }
    return settingsNum;
}

UserInputState PromptYN(const char *msg)
{
    string input;
    cout << msg;
    cin >> input;

    if (input == "N" || input == "n")
    {
        return NO;
    }
    else if (input == "Y" || input == "y")
    {
        return YES;
    }
    else
    {
        return INVALID;
    }
}