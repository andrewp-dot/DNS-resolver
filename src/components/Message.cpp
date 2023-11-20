/**
 * @file Message.cpp
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
 * @brief Implementation and function definitions of Message.h module.
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <arpa/inet.h>
#include "Message.h"
#include "Error.h"
#include "Query.h"
#include "constants.h"

#define CONVERT_TWO_CHARS_TO_UINT16(first, second) ((first << CHAR_BIT) | second)

static void convertBinaryToPrintable(std::vector<uint8_t> &printable, uint8_t binary)
{
    std::vector<uint8_t> reversedPrintable;
    if (binary == 0)
    {
        printable.push_back('0');
        return;
    }
    while (binary != 0)
    {
        // own function
        switch (binary % 10)
        {
        case 1:
            reversedPrintable.push_back('1');
            break;
        case 2:
            reversedPrintable.push_back('2');
            break;
        case 3:
            reversedPrintable.push_back('3');
            break;
        case 4:
            reversedPrintable.push_back('4');
            break;
        case 5:
            reversedPrintable.push_back('5');
            break;
        case 6:
            reversedPrintable.push_back('6');
            break;
        case 7:
            reversedPrintable.push_back('7');
            break;
        case 8:
            reversedPrintable.push_back('8');
            break;
        case 9:
            reversedPrintable.push_back('9');
            break;
        case 0:
            reversedPrintable.push_back('0');

        default:
            break;
        }
        binary = binary / 10;
    }

    for (size_t i = 0; i < reversedPrintable.size(); i++)
    {
        printable.push_back(reversedPrintable[reversedPrintable.size() - i - 1]);
    }
}

static void pushUINT8vectorToUINT8vector(std::vector<uint8_t> &dst, std::vector<uint8_t> &src)
{
    size_t i = 0;
    if (src[0] == '.')
    {
        i += 1;
    }
    for (; i < src.size(); i++)
    {
        dst.push_back(src[i]);
    }
    dst.push_back(',');
    // this is just for output formmating
    dst.push_back(' ');
}

static void pushUINT32ToUINT8vector(std::vector<uint8_t> &dst, uint32_t number)
{
    for (size_t i = 0; i < sizeof(number) / sizeof(uint8_t); i++)
    {
        uint8_t numberPart = (number >> (i * CHAR_BIT) & 0xff);
        dst.push_back(numberPart);
    }
}

uint16_t Message::generateQueryId(const Query &query)
{
    srand((unsigned)time(NULL));

    int range = 16;
    // random number from 0 to 16
    int id = (rand() % range);
    std::string addr = query.getAddress();
    return (uint16_t)(addr.size() << id);
}

QueryOpcode Message::getQueryOpcode(const Query &query)
{
    if (query.getReversed())
    {
        return OPCODE_IQUERY;
    }
    return OPCODE_QUERY;
}

DNSHeader Message::createHeader(const Query &query)
{
    DNSHeader header;
    std::memset(&header, 0, sizeof(struct DNSHeader));

    // set flags
    header.id = htons(generateQueryId(query));
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
    std::memcpy(&responseHeader, buffer, sizeof(struct DNSHeader));

    // improve uint16_t values
    responseHeader.id = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset], buffer[*offset + 1]);
    responseHeader.qdcount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 4], buffer[*offset + 5]);
    responseHeader.ancount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 6], buffer[*offset + 7]);
    responseHeader.nscount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 8], buffer[*offset + 9]);
    responseHeader.arcount = CONVERT_TWO_CHARS_TO_UINT16(buffer[*offset + 10], buffer[*offset + 11]);

    *offset += sizeof(struct DNSHeader);
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

std::vector<uint8_t> Message::getAddressFromResponse(char *buffer, uint16_t len, size_t *offset, uint16_t type)
{
    std::vector<uint8_t> res;
    for (size_t i = 0; i < len; i++)
    {
        if (type == A)
        {
            convertBinaryToPrintable(res, (uint8_t)buffer[*offset]);
            res.push_back('.');
        }
        else
        {
            res.push_back(buffer[*offset]);
        }
        *offset += 1;
    }
    // remove last '.'
    if (type == A)
    {
        res.pop_back();
    }
    return res;
}

std::vector<uint8_t> Message::getSoaFromResponse(char *buffer, size_t *offset)
{
    std::vector<uint8_t> rdata;

    // get email name
    std::vector<uint8_t> mname = getNameFromResponse(buffer, offset);
    *offset += 1;

    pushUINT8vectorToUINT8vector(rdata, mname);
    rdata.push_back(0);

    // get resource name
    std::vector<uint8_t> rname = getNameFromResponse(buffer, offset);
    *offset += 1;

    // push to vector and end it with zero
    pushUINT8vectorToUINT8vector(rdata, rname);
    rdata.push_back(0);

    // copy the soa info
    SOAinfo soaInfo;
    memcpy(&soaInfo, (buffer + *offset), sizeof(SOAinfo));

    // push uint32_T to uint8_vector
    soaInfo.serial = htonl(soaInfo.serial);
    pushUINT32ToUINT8vector(rdata, soaInfo.serial);
    // rdata.push_back(',');

    soaInfo.refresh = htonl(soaInfo.refresh);
    pushUINT32ToUINT8vector(rdata, soaInfo.refresh);
    // rdata.push_back(',');

    soaInfo.retry = htonl(soaInfo.retry);
    pushUINT32ToUINT8vector(rdata, soaInfo.retry);
    // rdata.push_back(',');

    soaInfo.expire = htonl(soaInfo.expire);
    pushUINT32ToUINT8vector(rdata, soaInfo.expire);
    // rdata.push_back(',');

    soaInfo.minimum = htonl(soaInfo.minimum);
    pushUINT32ToUINT8vector(rdata, soaInfo.minimum);

    return rdata;
}

std::vector<uint8_t> Message::getNameFromResponse(char *buffer, size_t *offset)
{
    std::vector<uint8_t> resName;

    // a pointer
    if (((uint8_t)buffer[*offset] & RESPONSE_POINTER_SIGN) == RESPONSE_POINTER_SIGN)
    {
        // mask the value out
        size_t pointerIndex = ((((uint16_t)buffer[*offset] << CHAR_BIT) | ((uint16_t)buffer[*offset + 1] & UINT8_MASK)) & RESPONSE_POINTER_MASK);
        *offset += 1;

        resName = getNameFromResponse(buffer, &pointerIndex);
        return resName;
    }
    else
    {
        int numberOfChars = buffer[*offset];
        // iterate until you find 0
        while (numberOfChars != 0)
        {
            *offset += 1;
            resName.push_back('.');

            // add numberOfChars labels
            for (size_t i = 0; i < (size_t)numberOfChars; i++)
            {
                resName.push_back(buffer[*offset + i]);
            }

            *offset += numberOfChars;
            numberOfChars = buffer[*offset];

            // ak je nasledujuci pocet rovny pointeru
            if (((uint8_t)buffer[*offset] & RESPONSE_POINTER_SIGN) == RESPONSE_POINTER_SIGN)
            {
                std::vector<uint8_t> resNameSuffix;
                resNameSuffix = getNameFromResponse(buffer, offset);

                for (size_t i = 0; i < resNameSuffix.size(); i++)
                {
                    resName.push_back(resNameSuffix[i]);
                }
                return resName;
            }
        }
        resName.push_back('.');
    }
    return resName;
}

DNSResponse Message::getResponse(char *buffer, size_t *offset)
{
    DNSResponse res;
    // memset(&res, 0, sizeof(struct DNSResponse));

    res.name = getNameFromResponse(buffer, offset);
    *offset += 1;

    // get response info
    DNSResponseInfo resInfo;

    // sizeof(struct DNSResponseInfo) - sizeof(uint16_t) bcs of implicit memory alignment
    memcpy(&resInfo, buffer + (*offset), sizeof(struct DNSResponseInfo) - sizeof(uint16_t));

    resInfo.type = htons(resInfo.type);
    resInfo.rclass = htons(resInfo.rclass);
    resInfo.ttl = htonl(resInfo.ttl);
    resInfo.rdlen = htons(resInfo.rdlen);
    res.info = resInfo;

    // move offset to rdata
    *offset += sizeof(struct DNSResponseInfo) - sizeof(uint16_t);

    // toto tu ziskat na zaklade rdlen
    if (resInfo.type == A || resInfo.type == AAAA)
    {

        res.rdata = getAddressFromResponse(buffer, resInfo.rdlen, offset, resInfo.type);
    }
    else if (resInfo.type == SOA)
    {
        res.rdata = getSoaFromResponse(buffer, offset);
    }
    else
    {
        res.rdata = getNameFromResponse(buffer, offset);
        *offset += 1;
    }
    return res;
}

void Message::printIPv6Address(std::vector<uint8_t> ip6)
{
    for (size_t i = 0; i < ip6.size(); i += 2)
    {
        uint16_t ip6Part = CONVERT_TWO_CHARS_TO_UINT16(ip6[i], ip6[i + 1]);
        std::cout << std::hex << ip6Part << std::dec;

        if (!(i % 2) && i + 2 != (ip6.size()))
        {
            std::cout << ":";
        }
    }
}

void Message::printSoaRecord(std::vector<uint8_t> vec)
{
    size_t recordOffset = 0;

    // print mname
    for (size_t i = 0; vec[i] != 0; i++)
    {
        std::cout << vec[recordOffset];
        recordOffset += 1;
    }
    // skip the zero
    recordOffset += 1;

    // print rname
    while (vec[recordOffset] != 0)
    {
        recordOffset += 1;
        std::cout << vec[recordOffset];
    }
    // skip the zero
    recordOffset += 1;

    // print other soa info
    uint32_t infoPart = 0;
    for (size_t i = 0; i < sizeof(SOAinfo) / sizeof(uint32_t); i++)
    {
        // convert uint32_t from 4 uint8_t
        infoPart = 0;
        for (size_t j = 0; j < sizeof(uint32_t); j++)
        {
            infoPart |= (uint32_t)(vec[recordOffset + j] << ((j % 4) * CHAR_BIT));
        }
        // skip the part value
        recordOffset += sizeof(uint32_t);
        std::cout << infoPart;
        if (i + 1 != sizeof(SOAinfo) / sizeof(uint32_t))
        {
            std::cout << ", ";
        }
    }
}

void Message::print8bitVector(std::vector<uint8_t> vec)
{
    // skip first dot
    size_t i = 0;
    if (vec[i] == '.')
    {
        i += 1;
    }

    // print rest of the string
    for (; i < vec.size(); i++)
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

void Message::printResponse(DNSResponse res)
{
    // print response based on type
    std::cout << "  ";
    if (res.name.size() != 1 && res.name[0] != 0)
    {
        print8bitVector(res.name);
        std::cout << ", ";
    }
    std::cout << convertTypeToString(res.info.type);
    std::cout << ", " << convertClassToString(res.info.rclass);
    std::cout << ", " << res.info.ttl;
    std::cout << ", ";
    if (res.info.type == AAAA)
    {
        printIPv6Address(res.rdata);
    }
    else if (res.info.type == SOA)
    {
        printSoaRecord(res.rdata);
    }
    else
    {
        print8bitVector(res.rdata);
    }
    std::cout << std::endl;
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
    std::memcpy(buffer, &this->header, sizeof(struct DNSHeader));

    // skip header
    buffer += sizeof(struct DNSHeader);
    convertSingleQuestionToBuffer(buffer, question);

    return sizeof(struct DNSHeader) + this->getQuestionSize();
}

void Message::parseResponseFromBuffer(char *buffer, int bufferSize)
{
    (void)bufferSize;
    // get header and question
    size_t offset = 0;
    DNSHeader responseHeader = getResponseHeader(buffer, &offset);
    DNSQuestion responseQuestion = getResponseQuestion(buffer, &offset);

    this->header = responseHeader;
    this->question = responseQuestion;

    // if response throws error
    if (this->header.rcode != SUCCESS)
    {
        Error::setErrorByResponseCode(this->header.rcode);
        return;
    }

    if (this->header.tc)
    {
        return;
    }

    // get responses
    std::vector<DNSResponse> responses;
    for (size_t i = 0; i < responseHeader.ancount; i++)
    {
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

    this->responses = responses;
}

void Message::printAnswers()
{
    size_t printedResponses = 0;
    std::cout << "Answer section (" << this->header.ancount << ")" << std::endl;
    for (size_t i = printedResponses; i < this->header.ancount; i++)
    {
        printResponse(this->responses[i]);
        printedResponses += 1;
    }

    size_t startIndex = printedResponses;
    std::cout << "Authority section (" << this->header.nscount << ")" << std::endl;
    for (size_t i = printedResponses; i < startIndex + this->header.nscount; i++)
    {
        printResponse(this->responses[i]);
        printedResponses += 1;
    }

    startIndex = printedResponses;
    std::cout << "Additional section (" << this->header.arcount << ")" << std::endl;
    for (size_t i = printedResponses; i < startIndex + this->header.arcount; i++)
    {
        printResponse(this->responses[i]);
        printedResponses += 1;
    }
}

void Message::printResponse()
{
    // header section
    std::cout << "Authoritative: " << convertBoolToString(this->header.aa) << ", ";
    std::cout << "Recursive: " << convertBoolToString(this->header.rd && this->header.ra) << ", ";
    std::cout << "Truncated: " << convertBoolToString(this->header.tc) << std::endl;

    // question section
    std::cout << "Question section (" << this->header.qdcount << ")" << std::endl;

    // function to ouptut uint8_t vector
    std::cout << "  ";
    print8bitVector(this->question.qname);
    std::cout << ", " << convertTypeToString(this->question.qtype);
    std::cout << ", " << convertClassToString(this->question.qclass) << std::endl;

    // print answers
    this->printAnswers();
}
