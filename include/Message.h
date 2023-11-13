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

enum QueryClass
{
    QCLASS_IN = 1
};

// #define QCLASS_IN 1 // internet QTYPE

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
    unsigned char qr : 1;     // query/response flagy

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

/*
                                    1  1  1  1  1  1
    0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                                               |
    /                                               /
    /                      NAME                     /
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TYPE                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                     CLASS                     |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                      TTL                      |
    |                                               |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
    |                   RDLENGTH                    |
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--|
    /                     RDATA                     /
    /                                               /
    +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
*/

typedef struct DNSResponseInfo
{
    uint16_t type;
    uint16_t rclass;
    uint32_t ttl;
    uint16_t rdlen;
} DNSResponseInfo;
typedef struct DNSResponse
{
    std::vector<uint8_t> name;
    DNSResponseInfo info;
    std::vector<uint8_t> rdata;

} DNSResponse;

class Message
{

private:
    DNSHeader header;
    DNSQuestion question;
    QueryType msgFormat;
    std::vector<DNSResponse> responses;
    int questionAmount;

    /**
     * @brief Generate query id
     *
     * @return unsigned short
     */
    unsigned short generateQueryId();

    /**
     * @brief Get the opcode of Query object
     *
     * @param query
     * @return QueryOpcode
     */
    QueryOpcode getQueryOpcode(const Query &query);

    /**
     * @brief Creates a message header
     *
     * @param query
     * @return DNSHeader
     */
    DNSHeader createHeader(const Query &query);

    /**
     * @brief converts address to labels in message
     *
     * @param addr
     * @param labels
     */
    void convertAddressToLabels(std::string addr, std::vector<uint8_t> &labels);

    /**
     * @brief Creates question from query
     *
     * @param query
     * @return DNSQuestion
     */
    DNSQuestion createQuestion(const Query &query);

    /**
     * @brief Returns the sum of size of class and size of type of the question
     *
     * @return size_t
     */
    inline size_t getQuestionTailSize() { return sizeof(this->question.qclass) + sizeof(this->question.qtype); };

    /**
     * @brief Returns sum of question qname and question tail
     *
     * @return size_t
     */
    size_t getQuestionSize() { return this->question.qname.size() + getQuestionTailSize(); }

    /**
     * @brief Convert one question to array of chars
     *
     * @param buffer
     * @param question
     */
    void convertSingleQuestionToBuffer(char *buffer, DNSQuestion &question);

    /**
     * @brief Get the Response Header object
     *
     * @param buffer
     * @param offset
     * @return DNSHeader
     */
    DNSHeader getResponseHeader(char *buffer, size_t *offset);

    /**
     * @brief Get the Response Question object
     *
     * @param buffer
     * @param offset
     * @return DNSQuestion
     */
    DNSQuestion getResponseQuestion(char *buffer, size_t *offset);

    /**
     * @brief Get the labels from given pointer index in buffer
     *
     */
    std::vector<uint8_t> getLabelsFromPointer(char *buffer, size_t *offset);

    /**
     * @brief Get the labels from given sequence in buffer
     *
     * @param buffer
     * @param offset
     * @return std::vector<uint8_t>
     */
    std::vector<uint8_t> getLabelsFromSequence(char *buffer, size_t *offset);

    /**
     * @brief Pushes data to response name of DNSResponse object. Set offset to point to end of the name
     *
     * @param res
     * @param buffer
     * @param offset
     */
    std::vector<uint8_t> getNameFromResponse(char *buffer, size_t *offset);

    /**
     * @brief Get the Response object
     *
     * @param buffer
     * @param offset
     * @return DNSResponse
     */
    DNSResponse getResponse(char *buffer, size_t *offset);

    /**
     * @brief Converts boolean value to string
     *
     * @param value - boolean to be entered
     * @return std::string - returns "Yes" if value is true, else returns "No"
     */
    inline std::string convertBoolToString(bool value) { return value ? "Yes" : "No"; };

    /**
     * @brief prints vector of uint8_t to the screen
     *
     * @param vec
     */
    void print8bitVector(std::vector<uint8_t> vec);

    /**
     * @brief converts query type to string
     *
     * @return std::string
     */
    std::string convertTypeToString(uint16_t qtype);

    /**
     * @brief Converts query class to string
     *
     * @return std::string
     */
    std::string convertClassToString(uint16_t qclass);

public:
    /**
     * @brief Construct a new Message object based on query
     *
     * @param query
     */
    Message(const Query &query);

    /**
     * @brief Converts message to array of chars
     *
     * @param buffer
     * @return size_t
     */
    size_t convertMsgToBuffer(char *buffer);

    /**
     * @brief Parses response to Message object
     *
     * @param buffer
     * @param bufferSize
     */
    void parseResponseToBuffer(char *buffer, int bufferSize);

    /**
     * @brief prints response in correct format
     *
     */
    void printResponse();
};

#endif
