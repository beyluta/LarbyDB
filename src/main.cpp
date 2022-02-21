#include "main.h"
// careful with changing these: things might break.
#define DEFAULT_PORT "8080"
#define DEFAULT_MANUAL_CONFIG false
#define DEFAULT_GENERATE_KEY true
#define DEFAULT_ALLOW_BACKUP true
#define DEFAULT_NUM_TABLES 2
#define DEFAULT_BACKUP_INTERVAL "30m"
#define DEFAULT_LOAD_BACKUP "ask"

using namespace std;

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
    string load_backup;
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
            if (line == "")
                continue;
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
            "#LarbyDB config\n# If set to true, will ask the user to input parameters on launch:\nmanual_config "+
            BoolToStr(DEFAULT_MANUAL_CONFIG)+
            "\n\n# sets the port:"+
            "\nport "+string(DEFAULT_PORT)+
            "\n\n# How many hashtables the db has (minimum 2):"+
            "\nnum_tables "+to_string(DEFAULT_NUM_TABLES)+
            "\n\n# Should the db perform automatic backups:\n# true | false\nallow_backup "+
            BoolToStr(DEFAULT_ALLOW_BACKUP)+
            "\n\n# Sets the time interval between performing backups:\n# h - hours\n# m - minutes\n# anything else counts as seconds.\n# Only positive integer values are allowed.\nbackup_interval "+
            string(DEFAULT_BACKUP_INTERVAL)+
            "\n\n# Generate the authentication key:\ngenerate_key "+
            BoolToStr(DEFAULT_GENERATE_KEY)+
            "\n\n# Should the db load from a backup file:\n# true  - load from backups\n# false - do not load from backups\n# ask   - ask the user whether to load from the backup file\nload_backup "+
            string(DEFAULT_LOAD_BACKUP)
            );
            return ProcessConfig();
        }
    }
    return args;
}

int ParseBackupInterval(Parameters& params, string& intervalStr)
{
    int result = 0;
    int interval = atoi(intervalStr.c_str());
    if (interval < 1)
    {
        string dflt = DEFAULT_BACKUP_INTERVAL;
        ParseBackupInterval(params, dflt);
    }
    else
    {
        result = 1;
        if (intervalStr.find("h") == intervalStr.size() - 1)
            params.backup_interval = interval * 3600;
        else if (intervalStr.find("m") == intervalStr.size() - 1)
            params.backup_interval = interval * 60;
        else
            params.backup_interval = interval;
    }
    return result;
}

//how many settings there are in total
#define NUM_SETTINGS 7
int SetParameters(Parameters& params, vector<string>& str)
{
    bool settings_set[NUM_SETTINGS] { false };
    enum {manual_config, port, num_tables, allow_backup, backup_interval, generate_key, load_backup };
    int settingsNum = 0;
    for (int i = 0; i < str.size() - 1; i++)
    {
        if (!settings_set[manual_config] && str[i].find("--manual_config") != string::npos)
        {
            settings_set[manual_config] = true;
            settingsNum++;
            if (str[i + 1] == "true")
                params.manual_config = true;
            else if (str[i + 1] == "false")
                params.manual_config = false;
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
            int result = ParseBackupInterval(params, str[i + 1]);
            if (result < 1)
                cout << "backup_interval: unset, using default (" << params.backup_interval << ")\n";
            settingsNum+= result;
            continue;
        }
        
        //TODO: improve these in the future
        if (!settings_set[generate_key] && str[i].find("--generate_key") != string::npos)
        {
            settings_set[generate_key] = true;
            settingsNum++;
            if (str[i + 1] == "true")
                params.generate_key = true;
            else if (str[i + 1] == "false")
                params.generate_key = false;
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
                params.allow_backup = true;
            else if (str[i + 1] == "false")
                params.allow_backup = false;
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

int PromptYN(const char* msg)
{
    string input;
    cout << msg;
    cin >> input;
    if (input == "N" || input == "n")
        return 0;
    else if (input == "Y" || input == "y")
        return 1;
    else
        return -1; //for invalid input
}

int main(int argc, char **argv)
{
    signal(SIGPIPE, SIG_IGN);
    Parameters dbParameters;
    {
        vector<string> configArguments = ProcessConfig();
        int configSettings = SetParameters(dbParameters, configArguments);
        if (NUM_SETTINGS - configSettings > 0)
            cout << "WARNING: config settings unset ("<< NUM_SETTINGS - configSettings << " out of "<< NUM_SETTINGS <<"), using fallback values.\n";
        
        if (argc > 1)
        {
            vector<string> launchArguments(argv + 1, argv + argc);
            SetParameters(dbParameters, launchArguments);
        }
    }

    if (dbParameters.manual_config)
    {
        // for boolean type settings
        auto PromptBool = [&] (const char* msg, bool& setting, bool defaultVal)
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
        // backup interval
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
    
    //TODO: there probably is a way to make the timers stack allocated. Look into that.
    Timer *backupTimer = new Timer();
    Timer *ttlTimer = new Timer();
    backupTimer->Subscribe(BackupHandlerMessage, 0);
    ttlTimer->Subscribe(TTLTimer, 0);

    //TODO: the controller needs to be global, same goes for the backuphandler.
    controller = new Controller(dbParameters.num_tables);
    {
        BackupHandler handler2(controller, true);
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
    const char* port = dbParameters.port.c_str();
    OnMessageReceived = &MessageReceived;
    Socket *socket = new Socket(port);

    if (dbParameters.allow_backup)
    {
        backupTimer->Start(dbParameters.backup_interval);
    }
    ttlTimer->Start(1);

    // fancy ascii art
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
    << (dbParameters.allow_backup ? to_string(dbParameters.backup_interval)+" seconds" : "unset")
    << '\n'
    << "generate_key: " << BoolToStr(dbParameters.generate_key) << '\n';

    if (dbParameters.generate_key)
    {
        controller->protectedByKey = true;
        controller->GenerateKey();
        std::cout << "Key: " << controller->hashtables[0].Get(DB_KEY_POSITION) << "\n";
    }
    else
    {
        std::cout << "WARNING: Running in unsafe mode. All commands will be accessible without a key!\n";
    }
    cout << "Database Initialized. " << std::endl;

    socket->Listen();

    delete socket;
    delete controller;
    delete backupTimer;
    delete ttlTimer;

    return 0;
}