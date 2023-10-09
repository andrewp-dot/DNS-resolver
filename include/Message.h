#ifndef MESSAGE_H
#define MESSAGE_H
#include "Query.h"

/**
 * @brief RR top level structure
 * NAME
 * TYPE
 * CLASS
 * TTL
 * RDLENGTH
 * RDATA
 */

/*
NAME            an owner name, i.e., the name of the node to which this
                resource record pertains.

TYPE            two octets containing one of the RR TYPE codes. (A, AAAA, PTR, ... )

CLASS           two octets containing one of the RR CLASS codes. (IN)

TTL             a 32 bit signed integer that specifies the time interval
                that the resource record may be cached before the source
                of the information should again be consulted.  Zero
                values are interpreted to mean that the RR can only be
                used for the transaction in progress, and should not be
                cached.  For example, SOA records are always distributed
                with a zero TTL to prohibit caching.  Zero values can
                also be used for extremely volatile data.

RDLENGTH        an unsigned 16 bit integer that specifies the length in
                octets of the RDATA field.

RDATA           a variable length string of octets that describes the
                resource.  The format of this information varies
                according to the TYPE and CLASS of the resource record.
*/

/* Messages:
    +---------------------+
    |        Header       |
    +---------------------+
    |       Question      | the question for the name server
    +---------------------+
    |        Answer       | RRs answering the question
    +---------------------+
    |      Authority      | RRs pointing toward an authority
    +---------------------+
    |      Additional     | RRs holding additional information
    +---------------------+
*/

#define QUERY 0
#define RESPONSE 1

enum QueryOpcode
{
    OPCODE_QUERY = 0,
    OPCODE_IQUERY = 1,
    OPCODE_SERVER_STATUS = 2
};

/*
                                |1  1  1  1  1  1
    |0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      ID                       |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |QR|   Opcode  |AA|TC|RD|RA|   Z    |   RCODE   |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    QDCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ANCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    NSCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                    ARCOUNT                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

typedef struct DNSHeader
{
    uint16_t id;              // identification number
    unsigned char rd : 1;     // recursion desired
    unsigned char tc : 1;     // truncated message
    unsigned char aa : 1;     // authoritative answer
    unsigned char opcode : 4; // purpose of message
    unsigned char qr : 1;     // query/response flagyy

    unsigned char rcode : 4; // response code
    unsigned char z : 3;     // its z! reserved
    unsigned char ra : 1;    // recursion available
    uint16_t qdcount;        // number of question entries
    uint16_t ancount;        // number of answer entries
    uint16_t nscount;        // number of authority entries
    uint16_t arcount;        // number of resource entries
} DNSHeader;

typedef struct DNSQuestion
{
    unsigned char dummy;
} DNSQuestion;

class Message
{

private:
    DNSHeader header;
    // question type
    QueryType msgFormat;

    unsigned short generateQueryId();
    DNSHeader createHeader(const Query &query);
    void createQuestion();

public:
    Message(const Query &query);
    void convertMsgToBuffer(char *buffer);
};

#endif
