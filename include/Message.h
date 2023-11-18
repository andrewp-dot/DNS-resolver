/**
 * @file Message.h
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
 * @brief Message module contatins information about sent message and recieved answer.
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef MESSAGE_H
#define MESSAGE_H
#include <vector>
#include "Query.h"

#define QUERY 0
#define RESPONSE 1

enum QueryOpcode
{
    OPCODE_QUERY = 0,
    OPCODE_IQUERY = 1,
    OPCODE_SERVER_STATUS = 2
};

enum QueryClass
{
    QCLASS_IN = 1
};

typedef struct SOATypeInfo
{
    uint32_t serial;
    uint32_t refresh;
    uint32_t retry;
    uint32_t expire;
    uint32_t minimum;
} SOAinfo;

typedef struct DNSHeader
{
    uint16_t id;              // identification number
    unsigned char rd : 1;     // recursion desired
    unsigned char tc : 1;     // truncated message
    unsigned char aa : 1;     // authoritative answer
    unsigned char opcode : 4; // purpose of message
    unsigned char qr : 1;     // query/response flags

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
    std::vector<uint8_t> qname; // sequence of labels; each label starts with byte, which represents length in chars of the label
    uint16_t qtype;             // 2 octet code which spicfices type of query
    uint16_t qclass;            // 2 octet code that specifies the class of the query
} DNSQuestion;

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
     * @return uint16_t
     */
    uint16_t generateQueryId(const Query &query);

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
     * @brief Get the address from  response buffer. Set the offset to pint on the last character of the name.
     *
     * @param buffer
     * @param len
     * @param offset
     * @param type
     * @return std::vector<uint8_t>
     */
    std::vector<uint8_t> getAddressFromResponse(char *buffer, uint16_t len, size_t *offset, uint16_t type);

    std::vector<uint8_t> getSoaFromResponse(char *buffer, size_t *offset);

    /**
     * @brief Pushes data to response name of DNSResponse object. Set offset to point to end of the name or a pointer
     *
     * @param res
     * @param buffer
     * @param offset
     */
    std::vector<uint8_t>
    getNameFromResponse(char *buffer, size_t *offset);

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
     * @brief prints SOA record in correct way
     *
     */
    void printSoaRecord(std::vector<uint8_t> vec);

    /**
     * @brief Prints IPv6 from vector
     *
     * @param vec
     */
    void printIPv6Address(std::vector<uint8_t> ip6);

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

    /**
     * @brief Prints given response
     *
     * @param res
     */
    void printResponse(DNSResponse res);

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
     * @brief Prints answered resources in correct format
     *
     */
    void printAnswers();

    /**
     * @brief Prints response in correct format
     *
     */
    void printResponse();
};

#endif
