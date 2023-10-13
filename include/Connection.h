#ifndef CONNECTION_H
#define CONNECTION_H
#include "Query.h"

enum ConnectionType
{
    UDP,
    TCP
};

class Connection
{
private:
    ConnectionType type;
    int sockfd;

    void setIPv6Settings();

public:
    Connection(ConnectionType type = UDP)
    {
        this->type = type;
    };
    void sendUdpQuery(const Query &query);
    void createTcpConnection();
};

#endif
