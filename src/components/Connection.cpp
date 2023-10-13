#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <net/init.h>
#include "constants.h"
#include "Connection.h"
#include "Message.h"
#include "Error.h"

#define UDP_LIMIT 255

/**
 * @brief
 * The resolver starts with knowledge of at least one name server.
 *
 * In return, the resolver either receives the desired
 * information or a referral to another name server.
 *
 * AUTHORITATIVE DATA
 * Name servers manage two kinds of data.  The first kind of data held in
 * sets called zones; each zone is the complete database for a particular
 * "pruned" subtree of the domain space.  This data is called
 * authoritative.
 */

/**
 * @brief Name grammar
 * <domain> ::= <subdomain> | " "
 * <subdomain> ::= <label> | <subdomain> "." <label>
 * <label> ::= <letter> [ [ <ldh-str> ] <let-dig> ]
 * <ldh-str> ::= <let-dig-hyp> | <let-dig-hyp> <ldh-str>
 * <let-dig-hyp> ::= <let-dig> | "-"
 * <let-dig> ::= <letter> | <digit>
 * <letter> ::= any one of the 52 alphabetic characters A through Z in upper case and a through z in lower case
 * <digit> ::= any one of the ten digits 0 through 9
 */

/*
 * DNS server:
 * kazi.fit.vutbr.cz
 * google : 8.8.8.8
 */

void Connection::sendUdpQuery()
{
    const char *googleDns = "8.8.8.8";

    // setup socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        Error::printError(CONNECTION_FAILED, "socket() failed\n");
        return;
    }

    // setup server address
    struct sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));

    server.sin_addr.s_addr = inet_addr(googleDns);
    server.sin_port = htons(53);
    server.sin_family = AF_INET;

    char buffer[UDP_DATAGRAM_LIMIT] = {
        0,
    };

    int bufferLength = (this->msg)->convertMsgToBuffer(buffer);

    int bytesTx = sendto(sockfd, (const char *)buffer, bufferLength, 0, (const sockaddr *)&server, sizeof(server));
    if (bytesTx < 0)
    {
        Error::printError(CONNECTION_FAILED, "sendto() failed\n");
        return;
    }

    close(sockfd);

    // // struct sockaddr_in server_addr;
    // struct sockaddr_in server_addr;

    // if ((this->sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    //     Error::printError(CONNECTION_FAILED, "socket() failed\n");
    // // throw std::runtime_error("creating new socket failed" + std::string(strerror(errno)));

    // server_addr.sin_family = AF_INET;
    // server_addr.sin_port = htons(10000);

    // if (inet_pton(AF_INET, googleDns, &server_addr.sin_addr) <= 0)
    //     Error::printError(CONNECTION_FAILED, "inet_pton() failed\n");
    // // throw std::runtime_error("converting to network fromat failed");

    // char buffer[UDP_DATAGRAM_LIMIT] = {
    //     0,
    // };

    // int bufferLength = (this->msg)->convertMsgToBuffer(buffer);

    // int bytesTx = sendto(this->sock, buffer, bufferLength, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    // if (bytesTx < 0)
    //     Error::printError(CONNECTION_FAILED, "sendto() failed\n");
    // throw std::runtime_error("sending msg failed" + std::string(strerror(errno)));

    // socklen_t len;
    // char response[UDP_LIMIT];
    // int bytesRead = recvfrom(this->sock, response, UDP_LIMIT, MSG_WAITALL, (struct sockaddr *)&server_addr, &len);

    // if (bytesRead < 0)
    //     Error::printError(CONNECTION_FAILED, "socket() failed\n");
    // throw std::runtime_error("recv of response failed" + std::string(strerror(errno)));

    close(this->sock);
}

// maybe
void Connection::createTcpConnection()
{
    std::cout << "TODO: create tcp connection " << std::endl;
}
