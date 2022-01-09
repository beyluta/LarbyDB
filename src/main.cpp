#include "main.h"

Controller *c;

const char *MessageReceived(const char *msg)
{
    return c->ResolveStringCommand(msg).c_str();
}

int main()
{
    char* port;
    std::cout << "Port number: ";
    std::cin >> port;

    int nTables;
    std::cout << "Number of hashtables in the database: ";
    std::cin >> nTables;

    while (std::cin.fail())
    {
        std::cin.clear();
        std::string emptyStr;
        std::getline(std::cin, emptyStr);
        std::cout << "Number of hashtables in the database: ";
        std::cin >> nTables;
    }
    c = new Controller(nTables);

    /* Creating the socket and listening for connections */
    OnMessageReceived = &MessageReceived;
    Socket *socket = new Socket(port);

    std::cout << "Database(" << nTables << "), port: " << port << "\n";
    
    socket->Listen();
    delete socket;
    delete c;

    return 0;
}