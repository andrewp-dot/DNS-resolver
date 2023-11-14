#ifndef CONNECTION_H
#define CONNECTION_H
#include "Query.h"
#include <arpa/inet.h>
#include <sys/socket.h>

class Connection
{
private:
    int sockfd;
    struct sockaddr_in server;

    /**
     * @brief Sets the settings for connection
     *
     */
    void connectionSetup(const Query &query);

    /**
     * @brief Sends query and displays recieved answer
     *
     */
    void sendAndDisplayAnswer(const Query &query);

    /**
     * @brief Closes the connection
     *
     */
    void connectionClose();

public:
    /**
     * @brief Construct a new Connection object
     *
     * @param type
     */
    Connection() { this->sockfd = -1; };

    /**
     * @brief Sends query using udp protocol
     *
     * @param query
     */
    void sendUdpQuery(const Query &query);
};

#endif
