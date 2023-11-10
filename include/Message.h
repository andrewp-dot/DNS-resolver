#ifndef MESSAGE_H
#define MESSAGE_H
#include "Query.h"

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

// enum QType
// {
//     IN = 1,
//     CS = 2,
//     CH = 3,
//     HS = 4,
// };

#define QCLASS_IN 1 // internet QTYPE

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

/*
QNAME           a domain name represented as a sequence of labels, where
                each label consists of a length octet followed by that
                number of octets.  The domain name terminates with the
                zero length octet for the null label of the root.  Note
                that this field may be an odd number of octets; no
                padding is used.

QTYPE           a two octet code which specifies the type of the query.
                The values for this field include all codes valid for a
                TYPE field, together with some more general codes which
                can match more than one type of RR.

QCLASS          a two octet code that specifies the class of the query.
                For example, the QCLASS field is IN for the Internet.
*/

typedef struct DNSQuestion
{
    std::vector<uint8_t> qname; // sequence of labels -> maybe array/ vector of octets // example 3 www 6 github 3 com 0
    uint16_t qtype;             // 2 octet code which spicfices type of query
    uint16_t qclass;            // a two octet code that specifies the class of the query.
} DNSQuestion;

class Message
{

private:
    DNSHeader header;
    // std::vector<DNSQuestion> questions;
    DNSQuestion question;
    QueryType msgFormat;
    int questionAmount;

    unsigned short generateQueryId();

    /* header */
    QueryOpcode getQueryOpcode(const Query &query);
    DNSHeader createHeader(const Query &query);

    /* questions */
    void convertAddressToLabels(std::string addr, std::vector<uint8_t> &labels);
    DNSQuestion createQuestion(const Query &query);
    // std::vector<DNSQuestion> createQuestions(const Query &query);

    /* supportive functions */
    // size_t qetQuestionTailSize() { return sizeof(this->questions[0].qclass) + sizeof(this->questions[0].qtype); }
    // size_t getQuestionSize(int index) { return this->questions[index].qname.size() + qetQuestionTailSize(); };
    size_t getQuestionTailSize()
    {
        return sizeof(this->question.qclass) + sizeof(this->question.qtype);
    };
    size_t getQuestionSize() { return this->question.qname.size() + getQuestionTailSize(); }
    size_t getDNSQuestionsSize();
    void convertSingleQuestionToBuffer(char *buffer, DNSQuestion &question);

    /* prints */
    void printMessageQnames();

public:
    Message(const Query &query);
    size_t convertMsgToBuffer(char *buffer);
};

#endif
