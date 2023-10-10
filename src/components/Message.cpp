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

std::vector<uint8_t> convertAddressToLabels(const Query &query)
{
    std::vector<uint8_t> labels;

    std::string addr = query.getAddress();

    size_t startPos = 0;

    while (startPos < addr.npos)
    {
        size_t endPos = addr.find(".", startPos);
        if (endPos == addr.npos)
        {
            endPos = addr.length();
        }
        // get substr from start to end
        std::string label = addr.substr(startPos, endPos - startPos);

        // insert label
        labels.push_back(label.length());
        for (char c : label)
        {
            labels.push_back(static_cast<uint8_t>(c));
        }
        startPos = endPos + 1;
    }
    labels.push_back(0);

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

void Message::convertMsgToBuffer(char *buffer)
{
    std::memcpy(buffer, &this->header, sizeof(DNSHeader));
    std::memcpy(buffer + sizeof(DNSHeader), &this->question, sizeof(DNSQuestion));

    // std::cout << (int)buffer[0] << " | " << (int)buffer[1] << std::endl;
    // std::cout << (int)buffer[2] << " | " << (int)buffer[3] << std::endl;
    // std::cout << (int)buffer[4] << " | " << (int)buffer[5] << std::endl;
    // std::cout << (int)buffer[6] << " | " << (int)buffer[7] << std::endl;
    // std::cout << (int)buffer[8] << " | " << (int)buffer[9] << std::endl;
    // std::cout << (int)buffer[10] << " | " << (int)buffer[11] << std::endl;
}
