#include "Message.h"
#include "Query.h"
#include <iostream>
#include "constants.h"

#define CONVERT_TWO_CHARS_TO_UINT16(first, second) ((first << CHAR_BIT) | second);

unsigned short Message::generateQueryId()
{
    // algorithm for generating query id
    // random number generator or hash function -> prefer hash function
    return 12345;
}

QueryOpcode Message::getQueryOpcode(const Query &query)
{
    if (query.getReversed())
    {
        // return OPCODE_IQUERY;
        return OPCODE_QUERY;
    }
    return OPCODE_QUERY;
}

DNSHeader Message::createHeader(const Query &query)
{
    DNSHeader header;
    std::memset(&header, 0, sizeof(DNSHeader));

    // set flags
    header.id = htons(generateQueryId());
    header.qr = QUERY;
    header.opcode = htons(getQueryOpcode(query));
    header.rd = query.getRecursionDesired();
    header.qdcount = htons(this->questionAmount);

    return header;
}

void Message::convertAddressToLabels(std::string addr, std::vector<uint8_t> &labels)
{
    size_t startPos = 0;

    while (startPos < addr.npos)
    {
        size_t endPos = addr.find(".", startPos);

        // get substr from start to end
        std::string label = addr.substr(startPos, endPos - startPos);

        // insert label
        labels.push_back(label.length());
        for (size_t i = 0; i < label.length(); i++)
        {
            labels.push_back(static_cast<uint8_t>(label[i]));
        }
        if (endPos >= addr.npos)
        {
            labels.push_back(0);
            break;
        }
        startPos = endPos + 1;
    }

    return;
}

DNSQuestion Message::createQuestion(const Query &query)
{
    DNSQuestion newQuestion;
    newQuestion.qclass = htons(QCLASS_IN);
    newQuestion.qtype = htons(query.getType());
    convertAddressToLabels(query.getAddress(), newQuestion.qname);
    return newQuestion;
}

size_t Message::getDNSQuestionsSize()
{
    return getQuestionSize();
}
void Message::convertSingleQuestionToBuffer(char *buffer, DNSQuestion &question)
{
    for (size_t i = 0; i < question.qname.size(); i++)
    {
        std::memcpy(buffer, &question.qname[i], sizeof(uint8_t));
        buffer += 1;
    }
    // qtype copy
    memcpy(buffer, &question.qtype, sizeof(uint16_t));
    buffer += sizeof(uint16_t);

    // qclass copy
    memcpy(buffer, &question.qclass, sizeof(uint16_t));
}

DNSHeader Message::getResponseHeader(char *buffer, size_t *offset)
{
    DNSHeader responseHeader;

    // copy flags etc
    std::memcpy(&responseHeader, buffer, sizeof(DNSHeader));

    // improve uint16_t values
    responseHeader.id = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset], buffer[*offset + 1]);
    responseHeader.qdcount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 4], buffer[*offset + 5]);
    responseHeader.ancount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 6], buffer[*offset + 7]);
    responseHeader.nscount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 8], buffer[*offset + 9]);
    responseHeader.arcount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 10], buffer[*offset + 11]);

    *offset += sizeof(DNSHeader);
    return responseHeader;
}

DNSQuestion Message::getResponseQuestion(char *buffer, size_t *offset)
{
    (void)buffer;
    DNSQuestion responseQuestion;

    // Note:: you will do this multiple times -> maybe function is required
    std::vector<uint8_t> qname;
    while (buffer[*offset] != 0)
    {
        qname.push_back(buffer[*offset]);
        *offset += 1;
    }
    qname.push_back(buffer[*offset]);
    *offset += 1;

    responseQuestion.qname = qname;

    responseQuestion.qtype = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset], buffer[*offset + 1]);
    *offset += sizeof(uint16_t);

    responseQuestion.qclass = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset], buffer[*offset + 1]);
    *offset += sizeof(uint16_t);
    return responseQuestion;
}

DNSResponse Message::getResponse(char *buffer, size_t *offset)
{
    // TODO: add next params of response
    DNSResponse res;
    memset(&res, 0, sizeof(DNSResponse));

    // check if incoming is a pointer
    if ((uint8_t)buffer[*offset] == RESPONSE_POINTER_SIGN)
    {
        size_t pointerIndex = (size_t)buffer[*offset + 1];
        printf("pointer | value: %d | char: %c\n", buffer[*offset + 1], buffer[pointerIndex + 1]);

        // loop until you find 00 -> maybe recursion in here
        res = getResponse(buffer, &pointerIndex);
    }
    else // loop until you find 00
    {
        std::vector<uint8_t> resName;
        while (buffer[*offset] != 0)
        {
            resName.push_back(buffer[*offset]);
            *offset += 1;
        }
        resName.push_back(buffer[*offset]);
        *offset += 1;

        res.name = resName;
    }
    return res;
}

Message::Message(const Query &query)
{
    this->msgFormat = query.getType();
    this->questionAmount = 1;
    this->header = createHeader(query);
    this->question = createQuestion(query);
}

size_t Message::convertMsgToBuffer(char *buffer)
{
    // header copy
    std::memcpy(buffer, &this->header, sizeof(DNSHeader));

    // skip header
    buffer += sizeof(DNSHeader);
    convertSingleQuestionToBuffer(buffer, question);

    return sizeof(DNSHeader) + this->getDNSQuestionsSize();
}

void Message::parseResponseToBuffer(char *buffer, int bufferSize)
{
    size_t offset = 0;
    DNSHeader responseHeader = getResponseHeader(buffer, &offset);
    DNSQuestion responseQuestion = getResponseQuestion(buffer, &offset);

    (void)responseHeader;
    // print qname, qtype, qclass
    // for (size_t i = 0; i < responseQuestion.qname.size(); i++)
    // {
    //     std::cout << "Label: " << responseQuestion.qname[i] << std::endl;
    // }

    // std::cout << "QTYPE" << responseQuestion.qtype << std::endl;
    // std::cout << "QCLASS" << responseQuestion.qclass << std::endl;

    DNSResponse response = getResponse(buffer, &offset);

    for (size_t i = 0; i < response.name.size(); i++)
    {
        std::cout << response.name[i] << std::endl;
    }
    // for (size_t i = offset; i < (size_t)bufferSize; i++)
    // {
    //     printf("%c : %d\n", (uint8_t)buffer[i], (uint8_t)buffer[i]);
    // }
    std::cout << "Buff size::" << bufferSize << std::endl;
    // parse data to format
}