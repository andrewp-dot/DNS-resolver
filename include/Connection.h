#ifndef CONNECTION_H
#define CONNECTION_H

#include "Message.h"

enum connectionType
{
    UDP,
    TCP
};

class Connection
{
private:
    connectionType type;
    int sock;
    Message *msg;

public:
    Connection(connectionType type = UDP)
    {
        this->type = type;
        this->msg = NULL;
    };
    // Message getMessage(Message &msg);
    void sendUdpQuery();
    void createTcpConnection();
};

#endif
