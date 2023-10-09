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

Message::Message(const Query &query)
{
    this->msgFormat = query.getType();
    this->header = createHeader(query);
    // question type
}

void Message::convertMsgToBuffer(char *buffer)
{
    // probably bad idea -> problem: dealing with endianes
    memcpy(buffer, &this->header, sizeof(this->header));
    std::cout << (int)buffer[0] << " | " << (int)buffer[1] << std::endl;
    std::cout << (int)buffer[2] << " | " << (int)buffer[3] << std::endl;
    std::cout << (int)buffer[4] << " | " << (int)buffer[5] << std::endl;
    std::cout << (int)buffer[6] << " | " << (int)buffer[7] << std::endl;
    std::cout << (int)buffer[8] << " | " << (int)buffer[9] << std::endl;
    std::cout << (int)buffer[10] << " | " << (int)buffer[11] << std::endl;
}
