/**
 * @file InputParser.cpp
 * @author Adrián Ponechal (xponec01@stud.fit.vut.cz)
 * @brief Implementation and function definitions of InputParser.h module
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <ctype.h>
#include <cstring>
#include "InputParser.h"
#include "Error.h"
#include "constants.h"

bool InputParser::isFlag(const char *arg)
{
    return arg[0] == '-';
}

InputParser::InputParser(int &argc, char **argv)
{
    this->argc = argc;
    this->argv = argv;
}

bool InputParser::isGoodPortFormat(const char *port)
{

    for (unsigned long i = 0; i < strlen(port); i++)
    {
        if (!isdigit(port[i]))
            return false;
    }

    unsigned long portNum = strtoul(port, NULL, 10);
    if ((long)portNum < 0 || portNum > MAX_PORT_NUMBER)
        return false;

    return true;
}

void InputParser::setQueryType(Query &query)
{
    if (query.getReversed())
    {
        query.setType(PTR);
    }
    else
    {
        if (query.getIPv6())
        {
            query.setType(AAAA);
        }
    }
}

void InputParser::modifyAddress(Query &query)
{

    if (query.getType() == PTR)
    {
        if (query.getIPv6())
        {
            query.reverseIPv6();
            query.setAddress(query.getAddress().append("ip6.arpa"));
        }
        else
        {
            query.reverseIPv4();
            query.setAddress(query.getAddress().append("in-addr.arpa"));
        }
    }
}

void InputParser::setOptions(Query &query)
{
    for (int i = 1; i < argc; i++)
    {
        if (isFlag(argv[i]))
        {
            if (strlen(argv[i]) != FLAG_LENGTH)
            {
                std::cerr << "Unknown flag: " << argv[i] << std::endl;
                // query.setIsOk(false);
                continue;
            }
            switch (argv[i][1])
            {
            case 'r':
                query.setRecursionDesired(true);
                break;
            case 'x':
                // maybe reversed is useless af
                query.setReversed(true);
                // query.setType(PTR);
                break;
            case '6':
                query.setIPv6(true);
                // query.setType(AAAA);
                break;
            case 's':
                i += 1;
                if (i >= argc)
                {
                    break;
                }
                if (isFlag(argv[i]))
                {
                    break;
                }
                else
                {
                    query.setServer(argv[i]);
                }
                break;
            case 'p':
                i += 1;
                if (i >= argc)
                {
                    // if port is wrong, set it to PORT 53
                    std::cerr << "Undefined option: " << argv[i] << "" << std::endl;
                    break;
                }
                if (isFlag(argv[i]))
                {
                    Error::printError(WRONG_ARGUMENTS, "Missing port.\n");
                    break;
                }
                if (isGoodPortFormat(argv[i]))
                {
                    query.setPort(atoi(argv[i]));
                }
                else
                {
                    Error::printError(WRONG_ARGUMENTS, "Invalid port. Port is set by default to %d.\n", DNS_PORT);
                }
                break;

            default:
                Error::printError(WRONG_ARGUMENTS, "Undefined option: %s\n", argv[i]);
                return;
            }
        }
        else
        {
            query.setAddress(argv[i]);
        }
    }

    setQueryType(query);
    modifyAddress(query);
}

void InputParser::parseArgs(Query &query)
{
    if (this->argc <= 1)
    {
        Error::printError(WRONG_ARGUMENTS, "Wrong number of arguments.\n");
        return;
    }
    setOptions(query);
    if (query.getServer() == "")
    {
        Error::printError(WRONG_ARGUMENTS, "Server has not been set.\n");
        return;
    }
    // if (query.getAddressVector().empty())
    if (query.getAddress() == "")
    {
        Error::printError(WRONG_ARGUMENTS, "Address has not been set.\n");
        return;
    }
}
