#ifndef CONNECTION_H
#define CONNECTION_H

#include "Message.h"

enum ConnectionType
{
    UDP,
    TCP
};

class Connection
{
private:
    ConnectionType type;
    int sock;
    Message *msg;

public:
    Connection(ConnectionType type = UDP, Message *msg)
    {
        this->type = type;
        this->msg = msg;
    };
    void sendUdpQuery();
    void createTcpConnection();
};

#endif
