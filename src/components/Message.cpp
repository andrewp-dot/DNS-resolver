#include "Message.h"
#include "Query.h"
#include <iostream>

DNSHeader Message::createHeader(const Query &query)
{
    DNSHeader header;
    memccpy(&header, NULL, 0, sizeof(DNSHeader));

    // set flags

    /*
    flags to set: id, qr, opcode, rd, qdcount - its number of addressses you want to ask for
    */
    // this->id = generateQueryId();
    // this–>qr = OPCODE_QUERY;
    // this->opcode = OPCODE_QUERY;

    // get all things together
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
    // just put all shit to buffer
}
