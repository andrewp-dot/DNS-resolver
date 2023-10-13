#include "Message.h"
#include "Query.h"
#include <iostream>

unsigned short Message::generateQueryId()
{
    // algorithm for generating query id
    // random number generator or hash function -> prefer hash function
    return 12345;
}

DNSHeader Message::createHeader(const Query &query)
{
    DNSHeader header;
    std::memset(&header, 0, sizeof(DNSHeader));

    // set flags
    header.id = htons(generateQueryId());
    header.qr = QUERY;
    header.opcode = htons(OPCODE_QUERY);
    header.rd = query.getRecursionDesired();
    header.qdcount = htons(1); // default 1 change it based on multiple querys

    return header;
}

std::vector<uint8_t> Message::convertAddressToLabels(const Query &query)
{
    std::vector<uint8_t> labels;

    std::string addr = query.getAddress();

    size_t startPos = 0;

    while (startPos < addr.npos)
    {
        size_t endPos = addr.find(".", startPos);

        // get substr from start to end
        std::string label = addr.substr(startPos, endPos - startPos); // here is an error

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

    return labels;
}

DNSQuestion Message::createQuestion(const Query &query)
{
    DNSQuestion question;
    question.qname = convertAddressToLabels(query);
    question.qclass = QTYPE_IN;
    question.qtype = query.getType();

    return question;
}

Message::Message(const Query &query)
{
    this->msgFormat = query.getType();
    this->header = createHeader(query);
    this->question = createQuestion(query);
}

size_t Message::convertMsgToBuffer(char *buffer)
{
    std::memcpy(buffer, &this->header, sizeof(DNSHeader)); // 12 bytes
    std::memcpy(buffer + sizeof(DNSHeader), &this->question, sizeof(DNSQuestion));
    return sizeof(DNSHeader) + sizeof(DNSQuestion);
}
