#include <iostream>
#include "Message.h"
#include "Query.h"
#include "constants.h"

#define CONVERT_TWO_CHARS_TO_UINT16(first, second) ((first << CHAR_BIT) | second);

void debugPrintChar(char c, int pos)
{
    std::cout << "\t\t\t\tc: " << c << " hex: " << std::hex << (uint16_t)c << " on position: " << std::dec << pos << std::endl;
}

void debugPrint(char *buffer, int bufferSize)
{
    for (size_t i = 0; i < (size_t)bufferSize; i++)
    {
        if (!(i % 8))
        {
            std::cout << "   ";
        }
        if (!(i % 16))
        {
            std::cout << std::endl;
        }
        std::cout << std::hex << (uint16_t)buffer[i];
        std::cout << " ";
    }
    std::cout << std::endl;
}

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

    std::vector<uint8_t> qname = getNameFromResponse(buffer, offset);

    responseQuestion.qname = qname;

    // get char after end of string/qname
    *offset += 1;
    responseQuestion.qtype = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset], buffer[*offset + 1]);
    *offset += sizeof(uint16_t);

    responseQuestion.qclass = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset], buffer[*offset + 1]);
    *offset += sizeof(uint16_t);
    return responseQuestion;
}

std::vector<uint8_t> Message::getLabelsFromPointer(char *buffer, size_t *offset)
{
    (void)buffer;
    (void)offset;
    std::vector<uint8_t> resName;
    // *offset += 1;
    // size_t pointerIndex = buffer[*offset];

    // int numberOfChars = buffer[pointerIndex];
    // pointerIndex += 1;
    // resName.push_back('.');

    // while (numberOfChars != 0)
    // {
    //     for (int i = 0; i < numberOfChars; i++)
    //     {
    //         resName.push_back(buffer[pointerIndex]);
    //         pointerIndex += 1;
    //     }
    //     numberOfChars = buffer[pointerIndex];
    //     resName.push_back('.');
    // }

    // std::cout << "Got from pointer: " << std::endl;
    // for (int i = 0; i < resName.size(); i++)
    // {
    //     printf("%c", resName[i]);
    // }
    // std::cout << std::endl;
    return resName;
}

std::vector<uint8_t> Message::getLabelsFromSequence(char *buffer, size_t *offset)
{
    (void)buffer;
    (void)offset;
    std::vector<uint8_t> resName;
    return resName;
}

std::vector<uint8_t> Message::getNameFromResponse(char *buffer, size_t *offset)
{
    static int numberOfPointers = 0;
    /**
     * TODO:
     * nastavit pointer aby pointoval pri konci funkcie na:
     * 1) c0 XY ...data...
     *       ^
     * 2) XY 00 ...data...
     *       ^
     * potom:
     * posunut offset na data
     *
     *
     */
    std::vector<uint8_t> resName;

    // a pointer
    if ((uint8_t)buffer[*offset] == RESPONSE_POINTER_SIGN)
    {

        // ani toto
        numberOfPointers += 1;
        *offset += 1;

        // TODO: vymaskovat ten index a upravit podmienku
        size_t pointerIndex = buffer[*offset];
        // zavolat rekurzivne funkciu; pointer je terat na
        // 1) c0 XY ...data...
        //       ^
        resName = getLabelsFromPointer(buffer, &pointerIndex);
        // *offset += 1;
        return resName;
    }
    else
    {
        int numberOfChars = buffer[*offset];
        // posunutie pointeru na dalsi prvok

        // iterovat cez labely az dokym nenarazis na 00
        while (numberOfChars != 0)
        {
            // miesto cisla pushni bodku
            *offset += 1;
            resName.push_back('.');
            // pridavat numberOfChars labelov
            for (size_t i = 0; i < (size_t)numberOfChars; i++)
            {
                resName.push_back(buffer[*offset + i]);
            }

            *offset += numberOfChars;
            numberOfChars = buffer[*offset];

            // ak je nasledujuci pocet rovny pointeru,

            // toto sa nikdy nestane
            if ((uint8_t)buffer[*offset] == RESPONSE_POINTER_SIGN)
            {
                numberOfPointers += 1;
                *offset += 1;

                // TODO: vymaskovat ten index a upravit podmienku
                size_t pointerIndex = buffer[*offset];
                // zavolaj rekurzivne funkciu a pridaj cast labelu do resName
                std::vector<uint8_t> resNameSuffix;
                resNameSuffix = getNameFromResponse(buffer, &pointerIndex);

                for (size_t i = 0; i < resNameSuffix.size(); i++)
                {
                    resName.push_back(resNameSuffix[i]);
                }
                // *offset += 1;
                return resName;
            }
            // *offset += 1; // get next char
        }
        // na konci stringu posun offseet o 1
        // *offset += 1;
        resName.push_back('.');

        // pointer teraz ukazuje tu.
        // 2) XY 00 ...data...
        //       ^
    }

    // pointer ukazujuci na data
    std::cout << "number of pointers: " << numberOfPointers << std::endl;
    return resName;
}

DNSResponse Message::getResponse(char *buffer, size_t *offset)
{
    DNSResponse res;
    memset(&res, 0, sizeof(DNSResponse));

    debugPrintChar(buffer[*offset], (int)*offset);
    res.name = getNameFromResponse(buffer, offset);
    *offset += 1;

    // get response info
    DNSResponseInfo resInfo;
    memcpy(&resInfo, buffer + (*offset), sizeof(DNSResponseInfo) - sizeof(uint16_t));

    resInfo.type = htons(resInfo.type);
    resInfo.rclass = htons(resInfo.rclass);
    resInfo.ttl = htonl(resInfo.ttl);
    resInfo.rdlen = htons(resInfo.rdlen);
    res.info = resInfo;

    // move offset to rdata
    *offset += sizeof(DNSResponseInfo) - sizeof(uint16_t);

    debugPrintChar(buffer[*offset], (int)*offset);
    res.rdata = getNameFromResponse(buffer, offset);
    *offset += 1;
    return res;
}

void Message::print8bitVector(std::vector<uint8_t> vec)
{
    // skip first dot
    if (vec[0] == '.')
    {
        std::cout << " ";
    }

    // print rest of the string
    for (size_t i = 1; i < vec.size(); i++)
    {
        std::cout << vec[i];
    }
}

std::string Message::convertTypeToString(uint16_t qtype)
{
    switch (qtype)
    {
    case A:
        return "A";
    case NS:
        return "NS";
    case CNAME:
        return "CNAME";
    case SOA:
        return "SOA";
    case PTR:
        return "PTR";
    case AAAA:
        return "AAAA";
    default:
        return "UNKNOWN";
    }
}

std::string Message::convertClassToString(uint16_t qclass)
{
    switch (qclass)
    {
    case QCLASS_IN:
        return "IN";

    default:
        return "UNKNOWN";
    }
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

    return sizeof(DNSHeader) + this->getQuestionSize();
}

void Message::parseResponseToBuffer(char *buffer, int bufferSize)
{
    (void)bufferSize;
    // get header and question
    size_t offset = 0;
    DNSHeader responseHeader = getResponseHeader(buffer, &offset);
    DNSQuestion responseQuestion = getResponseQuestion(buffer, &offset);

    this->header = responseHeader;
    this->question = responseQuestion;

    // get responses
    std::vector<DNSResponse> responses;
    for (size_t i = 0; i < responseHeader.ancount; i++)
    {
        // printf("Before response offset: %d\n", (int)offset);
        DNSResponse res = getResponse(buffer, &offset);
        responses.push_back(res);
    }

    for (size_t i = 0; i < responseHeader.nscount; i++)
    {
        DNSResponse res = getResponse(buffer, &offset);
        responses.push_back(res);
    }

    for (size_t i = 0; i < responseHeader.arcount; i++)
    {
        DNSResponse res = getResponse(buffer, &offset);
        responses.push_back(res);
    }

    for (size_t resID = 0; resID < responses.size(); resID++)
    {
        std::cout << "Response: " << resID << std::endl;
        for (size_t i = 0; i < responses[resID].name.size(); i++)
        {
            printf("%c", responses[resID].name[i]);
        }
        std::cout << std::endl;
        std::cout << "Rtype:  " << responses[resID].info.type << std::endl;
        std::cout << "Rclass: " << responses[resID].info.rclass << std::endl;
        std::cout << "ttl:    " << responses[resID].info.ttl << std::endl;
        std::cout << "Rdlen:  " << responses[resID].info.rdlen << std::endl;
        std::cout << std::endl;

        std::cout << "Rdata: " << resID << std::endl;
        for (size_t i = 0; i < responses[resID].rdata.size(); i++)
        {
            printf("%c", responses[resID].rdata[i]);
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    // for (size_t i = 0; i < (size_t)bufferSize; i++)
    // {
    //     printf("%d: c: %c\n", (int)i, buffer[i]);
    // }

    std::cout << bufferSize << std::endl;
    // debugPrint(buffer, bufferSize);
}

void Message::printResponse()
{
    // header section
    std::cout << "Authoritative: " << convertBoolToString(this->header.aa) << ", ";
    std::cout << "Recursive: " << convertBoolToString(this->header.rd) << ", ";
    std::cout << "Truncated: " << convertBoolToString(this->header.tc) << std::endl;

    // question section
    std::cout << "Question section (" << this->header.qdcount << ")" << std::endl;

    // function to ouptut uint8_t vector
    print8bitVector(this->question.qname);
    std::cout << "," << convertTypeToString(this->question.qtype);
    std::cout << "," << convertClassToString(this->question.qclass) << std::endl;
}
