#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "constants.h"
#include "Connection.h"
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

void Connection::sendUdpQuery()
{
    if ((this->sock = socket(AF_INET, SOCK_DGRAM, 0) == -1))
    {
        Error::printError(CONNECTION_FAILED, "socket() failed\n");
        return;
    }

    struct sockaddr_in server, from; // adresova struktura serveru a klienta

    server.sin_addr.s_addr = inet_addr("8.8.8.8");
    server.sin_family = AF_INET;
    server.sin_port = DNS_PORT; // get port - returns query port, or if it is unset, returns DNS_PORT

    if (connect(this->sock, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        Error::printError(CONNECTION_FAILED, "connect() failed\n");
        return;
    } // nastaveni spojovane UDP schranky

    char buffer[UDP_DATAGRAM_LIMIT] = {
        0,
    };
    // int bufferLength = setupMessage(buffer);
    int bufferLength = 10;

    if (send(sock, buffer, bufferLength, 0) != bufferLength)
    {
        Error::printError(CONNECTION_FAILED, "send() failed\n");
    }

    socklen_t fromlen;
    if (recv(sock, buffer, UDP_DATAGRAM_LIMIT, 0) > 0)
    {                                                          // ˇcten´ı dat od serveru
        getpeername(sock, (struct sockaddr *)&from, &fromlen); // IP adresa a port serveru
        printf("data received from %s, port %d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
    }
    close(this->sock);
}

// maybe
void Connection::createTcpConnection()
{
    std::cout << "TODO: create tcp connection " << std::endl;
}
