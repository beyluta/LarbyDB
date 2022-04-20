#include "file.h"

using namespace FileUtility;

#define DEFAULT_PORT "8080"
#define DEFAULT_MANUAL_CONFIG false
#define DEFAULT_GENERATE_KEY true
#define DEFAULT_ALLOW_BACKUP true
#define DEFAULT_NUM_TABLES 2
#define DEFAULT_BACKUP_INTERVAL "30m"
#define DEFAULT_LOAD_BACKUP "ask"
#define DEFAULT_NUM_SETTINGS 7
#define SEMANTIC_VERSION "v0.2.0"

struct Parameters
{
    string port;
    int num_tables;
    bool generate_key;
    bool allow_backup;
    bool manual_config;
    int backup_interval;
    string load_backup;
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
                                     "#LarbyDB config\n# If set to true, will ask the user to input parameters on launch:\nmanual_config " +
                                         BoolToStr(DEFAULT_MANUAL_CONFIG) +
                                         "\n\n# sets the port:" +
                                         "\nport " + string(DEFAULT_PORT) +
                                         "\n\n# How many hashtables the db has (minimum 2):" +
                                         "\nnum_tables " + to_string(DEFAULT_NUM_TABLES) +
                                         "\n\n# Should the db perform automatic backups:\n# true | false\nallow_backup " +
                                         BoolToStr(DEFAULT_ALLOW_BACKUP) +
                                         "\n\n# Sets the time interval between performing backups:\n# h - hours\n# m - minutes\n# anything else counts as seconds.\n# Only positive integer values are allowed.\nbackup_interval " +
                                         string(DEFAULT_BACKUP_INTERVAL) +
                                         "\n\n# Generate the authentication key:\ngenerate_key " +
                                         BoolToStr(DEFAULT_GENERATE_KEY) +
                                         "\n\n# Should the db load from a backup file:\n# true  - load from backups\n# false - do not load from backups\n# ask   - ask the user whether to load from the backup file\nload_backup " +
                                         string(DEFAULT_LOAD_BACKUP));
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
        load_backup
    };
    int settingsNum = 0;

    for (int i = 0; i < str.size() - 1; i++)
    {
        if (!settings_set[manual_config] && str[i].find("--manual_config") != string::npos)
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
                cout << boolalpha << "manual_config: unset, using default (" << params.manual_config << ")\n";
            }
            continue;
        }

        if (!settings_set[port] && str[i].find("--port") != string::npos)
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
                cout << "port: unset, using default (" << params.port << ")\n";
            }
            continue;
        }

        if (!settings_set[num_tables] && str[i].find("--num_tables") != string::npos)
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
                cout << "num_tables: unset, using default (" << params.num_tables << ")\n";
            }
            continue;
        }

        if (!settings_set[backup_interval] && str[i].find("--backup_interval") != string::npos)
        {
            settings_set[backup_interval] = true;
            ParseBackupInterval(params, str[i + 1]);
            settingsNum++;
            continue;
        }

        if (!settings_set[generate_key] && str[i].find("--generate_key") != string::npos)
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
                cout << boolalpha << "generate_key: unset, using default (" << params.generate_key << ")\n";
            }
            continue;
        }

        if (!settings_set[allow_backup] && str[i].find("--allow_backup") != string::npos)
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
                cout << boolalpha << "allow_backup: unset, using default (" << params.allow_backup << ")\n";
            }
            continue;
        }

        if (!settings_set[load_backup] && str[i].find("--load_backup") != string::npos)
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
                cout << "allow_backup: unset, using default (" << params.load_backup << ")\n";
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