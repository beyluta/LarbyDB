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

int main()
{
    Timer *backupTimer = new Timer();
    backupTimer->Subscribe(BackupHandler, 0);

    char *port;
    std::cout << "Port number: ";
    std::cin >> port;

    int nTables;
    std::cout << "Number of tables in the database: ";
    std::cin >> nTables;

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

    std::cout << "Database Tables Initialized(" << nTables << "), port: " << port << "\n";

    backupTimer->Start(1);
    socket->Listen();
    delete socket;
    delete controller;
    delete backupTimer;

    return 0;
}