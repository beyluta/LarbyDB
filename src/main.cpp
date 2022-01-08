#include "main.h"

Controller *c;

const char *MessageReceived(const char *msg)
{
    return c->ResolveStringCommand(msg).c_str();
}

int main()
{
    /* Creating the controller and all the tables of the database
    *  This should later be replaced with a function to ask for: 
    *  Number of tables, which port to run the service on.*/
    c = new Controller(2);

    /* Creating the socket and listening for connections */
    OnMessageReceived = &MessageReceived;
    Socket *socket = new Socket("8000");
    socket->Listen();
    delete socket;
    delete c;

    // Controller c(2);
    // c.ResolveStringCommand("INSERT /<3 Larry/ INTO /0/ WHEREKEY /larry/");
    // std::cout << c.ResolveStringCommand("FETCH /larry/ FROM /0/") << std::endl;
    return 0;
}