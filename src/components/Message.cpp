#include "Message.h"
#include "Query.h"
#include <iostream>

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
        return OPCODE_IQUERY;
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

// std::vector<DNSQuestion> Message::createQuestions(const Query &query)
// {
//     std::vector<DNSQuestion> questions; // undefined
//     questions.reserve(this->questionAmount);

//     int i = 0;
//     for (auto addr : query.getAddressVector())
//     {
//         questions[i].qclass = htons(QCLASS_IN);
//         questions[i].qtype = htons(query.getType());

//         // based on query settings set up question
//         // if (query.getReversed())
//         // {
//         //     printf("reversed\n");
//         // }
//         // else
//         // {
//         //     this->convertAddressToLabels(addr, questions[i].qname);
//         // }
//         this->convertAddressToLabels(addr, questions[i].qname);
//         i += 1;
//     }

//     return questions;
// }

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
    std::cout << "Question" << std::endl;
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
    buffer += sizeof(uint16_t);
}

size_t Message::getDNSQuestionsSize()
{
    // size_t questionsSize = 0;
    // for (int i = 0; i < this->questionAmount; i++)
    // {
    //     questionsSize += getQuestionSize(i);
    // }
    // return questionsSize;
    return getQuestionSize();
}

// void Message::printMessageQnames()
// {
//     for (int i = 0; i < this->questionAmount; i++)
//     {
//         for (size_t j = 0; j < questions[i].qname.size(); j++)
//         {
//             if (this->questions[i].qname[j] < 32)
//             {
//                 std::cout << " ";
//             }
//             else
//                 std::cout << this->questions[i].qname[j];
//         }
//         std::cout << "|" << std::endl;
//     }
// }

Message::Message(const Query &query)
{
    this->msgFormat = query.getType();
    // this->questionAmount = query.getAddressVector().size();
    this->questionAmount = 1;
    this->header = createHeader(query);
    this->question = createQuestion(query);
    // this->questions = createQuestions(query);
    // printMessageQnames();
}

size_t Message::convertMsgToBuffer(char *buffer)
{
    // header copy
    std::memcpy(buffer, &this->header, sizeof(DNSHeader));

    // skip header
    buffer += sizeof(DNSHeader);

    // qname(s) copy

    // multiquestion solution
    // int questionNum = 0;
    // while (questionNum < this->questionAmount)
    // {
    //     convertSingleQuestionToBuffer(buffer, questions[questionNum]);
    //     buffer += this->getQuestionSize(questionNum);
    //     questionNum += 1;
    // }
    convertSingleQuestionToBuffer(buffer, question);

    return sizeof(DNSHeader) + this->getDNSQuestionsSize();
}

/*
| HEADER | QUESTION1 | Q1TAIL | QUESTION2 | Q2TAIL | ... | QUESTIONX | QX TAIL |
*/
