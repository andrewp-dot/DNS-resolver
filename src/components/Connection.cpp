#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include "constants.h"
#include "Connection.h"
#include "Message.h"
#include "Error.h"

void Connection::connectionSetup(const Query &query)
{
    // setup socket
    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        Error::printError(CONNECTION_FAILED, "socket() failed\n");
        return;
    }

    int flags = fcntl(this->sockfd, F_GETFL, 0);
    fcntl(this->sockfd, F_SETFL, flags | O_NONBLOCK);

    // setup server address
    memset(&this->server, 0, sizeof(sockaddr_in));
    // memset(&this->from, 0, sizeof(sockaddr_in));

    /**
     * TODO:
     * use get host by name here
     */
    this->server.sin_addr.s_addr = inet_addr(query.getServer().c_str());
    this->server.sin_port = htons(query.getPort());
    this->server.sin_family = AF_INET;
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

    // timeout #DOC
    fd_set readSet;
    FD_ZERO(&readSet);              /* clear the set */
    FD_SET(this->sockfd, &readSet); /* add our file descriptor to the set */

    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    int rv = select(this->sockfd + 1, &readSet, NULL, NULL, &timeout);

    if (rv < 0)
    {
        Error::printError(CONNECTION_FAILED, "SOCKET_ERROR\n");
        return;
    }
    else if (rv == 0)
    {
        Error::printError(CONNECTION_FAILED, "Invalid server\n");
        return;
    }
    else
    {
        int bytesRx = recv(this->sockfd, (char *)recvBuffer, UDP_DATAGRAM_LIMIT, 0);
        if (bytesRx < 0)
        {
            Error::printError(CONNECTION_FAILED, "recv() failed\n");
            return;
        }

        // parse connection to buffer
        msg.parseResponseToBuffer(recvBuffer, bytesRx);
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
    connectionSetup(query);
    sendAndDisplayAnswer(query);
    connectionClose();
}
