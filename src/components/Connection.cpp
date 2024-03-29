/**
 * @file Connection.cpp
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
 * @brief Implementation and function definitions of Connection.h functions
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include "constants.h"
#include "Connection.h"
#include "Message.h"
#include "Error.h"

bool Connection::getHostIPaddress(const char *server, struct in_addr *dst)
{
    struct hostent *hostInfo;

    // check if host was found
    hostInfo = gethostbyname(server);
    if (hostInfo == NULL)
    {
        Error::printError(CONNECTION_FAILED, "Host IP address not found.\n");
        return false;
    }

    // get ip address
    struct in_addr **addressList = (struct in_addr **)hostInfo->h_addr_list;
    if (addressList == NULL)
    {
        Error::printError(CONNECTION_FAILED, "Host IP address not found.\n");
        return false;
    }

    // iterate through address list
    for (size_t i = 0; addressList[i] != NULL; i++)
    {
        char *ipAddr = inet_ntoa(*addressList[i]);

        // inet_pton() returns 1 on success
        if (inet_pton(AF_INET, ipAddr, &dst->s_addr) < 1)
        {
            // if there is no more ip address in list, get error
            if (addressList[i + 1] == NULL)
            {
                Error::printError(CONNECTION_FAILED, "Host IP address not found.\n");
                return false;
            }
        }
        // found
        else
        {
            break;
        }
    }

    return true;
}

bool Connection::connectionSetup(const Query &query)
{
    // setup socket
    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        Error::printError(CONNECTION_FAILED, "socket() failed\n");
        return false;
    }

    // setup server address
    memset(&this->server, 0, sizeof(sockaddr_in));

    struct in_addr serverAddr;
    if (!getHostIPaddress(query.getServer().c_str(), &serverAddr))
    {
        return false;
    }

    this->server.sin_addr = serverAddr;
    this->server.sin_port = htons(query.getPort());
    this->server.sin_family = AF_INET;

    return true;
}

void Connection::sendAndDisplayAnswer(const Query &query)
{
    if (connect(this->sockfd, (struct sockaddr *)(&this->server), sizeof(this->server)) < 0)
    {
        Error::printError(CONNECTION_FAILED, "connect() failed\n");
        return;
    }

    char buffer[UDP_DATAGRAM_LIMIT] = {0};

    Message msg = Message(query);
    int bufferLength = msg.convertMsgToBuffer(buffer);

    int bytesTx = send(this->sockfd, (const char *)buffer, bufferLength, 0);
    if (bytesTx < 0)
    {
        Error::printError(CONNECTION_FAILED, "send() failed\n");
        return;
    }

    // recieve message
    char recvBuffer[UDP_DATAGRAM_LIMIT] = {0};

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    setsockopt(this->sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    int bytesRx = recv(this->sockfd, (char *)recvBuffer, UDP_DATAGRAM_LIMIT, 0);
    if (bytesRx < 0)
    {
        Error::printError(CONNECTION_FAILED, "recv() failed\n");
        return;
    }

    // parse connection to buffer
    msg.parseResponseFromBuffer(recvBuffer, bytesRx);
    if (Error::getErrorCode() == SUCCESS)
    {
        msg.printResponse();
    }
}

void Connection::connectionClose()
{
    close(this->sockfd);
}

// #DOC
void Connection::sendUdpQuery(const Query &query)
{
    if (!connectionSetup(query))
    {
        return;
    }
    sendAndDisplayAnswer(query);
    connectionClose();
}
