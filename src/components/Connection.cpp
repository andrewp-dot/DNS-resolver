#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "constants.h"
#include "Connection.h"
#include "Message.h"
#include "Error.h"

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

void Connection::sendUdpQuery(const Query &query)
{

    // setup socket
    this->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        Error::printError(CONNECTION_FAILED, "socket() failed\n");
        return;
    }

    // setup server address
    struct sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));

    server.sin_addr.s_addr = inet_addr(query.getServer().c_str());
    server.sin_port = htons(query.getPort());
    server.sin_family = AF_INET;

    char buffer[UDP_DATAGRAM_LIMIT] = {
        0,
    };

    Message msg = Message(query);
    int bufferLength = msg.convertMsgToBuffer(buffer);

    int bytesTx = sendto(this->sockfd, (const char *)buffer, bufferLength, 0, (const sockaddr *)&server, sizeof(server));
    if (bytesTx < 0)
    {
        Error::printError(CONNECTION_FAILED, "sendto() failed\n");
        return;
    }

    close(this->sockfd);
}

// maybe
void Connection::createTcpConnection()
{
    std::cout << "TODO: create tcp connection " << std::endl;
}
