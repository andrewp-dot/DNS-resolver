/**
 * @file Connection.h
 * @author Adrián Ponechal (xponec01@stud.fit.vut.cz)
 * @brief Module for communication processing
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CONNECTION_H
#define CONNECTION_H
#include "Query.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

class Connection
{
private:
    int sockfd;
    struct sockaddr_in server;

    /**
     * @brief Get the ip address of host
     *
     * @param server
     * @return true - if the address is found
     * @return false - if the address is not found
     */
    bool getHostIPaddress(const char *server, struct in_addr *dst);

    /**
     * @brief Sets the settings for connection
     *
     * @param query
     * @return true - if conection was set up correctly
     * @return false - if there is an error
     */
    bool connectionSetup(const Query &query);

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
