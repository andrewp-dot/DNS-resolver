/**
 * @brief  Dns resolver for course ISA on VUT BUT Brno. It resolves address of specified server.
 * Copyright (C) 2023  xponec01@stud.fit.vutbr.cz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <string>
#include <ctype.h>
#include "InputParser.h"
#include "Error.h"
#include "constants.h"

/**
 * @brief checks if next program argument is a flag
 *
 * @return true
 * @return false
 */
bool isFlag(const char *arg)
{

    return arg[0] == '-';
}

/**
 * @brief Construct a new Input Parser:: Input Parser object
 *
 * @param argc - number of program arguments
 * @param argv - program arguments array
 */

InputParser::InputParser(int &argc, char **argv)
{
    this->argc = argc;
    this->argv = argv;
}

/**
 * @brief Checks string port format
 *
 * @param port - port to be verified
 * @return true
 * @return false
 */

bool InputParser::isGoodPortFormat(const char *port)
{

    for (unsigned long i = 0; i < strlen(port); i++)
    {
        if (!isdigit(port[i]))
            return false;
    }

    unsigned long portNum = strtoul(port, NULL, 10);
    if (portNum < 0 || portNum > MAX_PORT_NUMBER)
        return false;

    return true;
}

/**
 * @brief Set options of given query based on given arguments
 *
 * @param query
 */
void InputParser::setOptions(Query &query)
{
    for (int i = 1; i < argc; i++)
    {
        if (isFlag(argv[i]))
        {
            if (strlen(argv[i]) != FLAG_LENGTH)
            {
                std::cerr << "Unknown flag: " << argv[i] << std::endl;
                query.setIsOk(false);
                continue;
            }
            switch (argv[i][1])
            {
            case 'r':
                query.setRecursionDesired(true);
                break;
            case 'x':
                query.setReversed(true);
                break;
            case '6':
                query.setType(AAAA);
                break;
            case 's':
                i += 1;
                if (i >= argc)
                {
                    query.setIsOk(false);
                    break;
                }
                if (isFlag(argv[i]))
                {
                    query.setIsOk(false);
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
                    query.setIsOk(false);
                    break;
                }
                if (isFlag(argv[i]))
                {
                    Error::printError(WRONG_ARGUMENTS, "Missing port.\n");
                    query.setIsOk(false);
                    break;
                }
                if (isGoodPortFormat(argv[i]))
                {
                    query.setPort(atoi(argv[i]));
                }
                else
                {
                    Error::printError(WRONG_ARGUMENTS, "Invalid port. Port is set by default to %d.\n", DNS_PORT);
                    query.setIsOk(false);
                }
                break;

            default:
                Error::printError(WRONG_ARGUMENTS, "Undefined option: %s\n", argv[i]);
                return;
            }
        }
        else
        {
            query.pushAddress(argv[i]);
        }
    }
}

/**
 * @brief checks IPv4 address format
 *
 * @param address
 * @return int
 */
void InputParser::checkIPv4AddressType(std::string address)
{
    if (address.length() > IPV4_MAX_LENGTH || address.length() < IPV4_MIN_LENGTH)
    {
        const char *msgSpecification = address.length() > IPV4_MAX_LENGTH ? "long" : "short";
        Error::printError(IPV4_WRONG_FORMAT, "IPv4 address is too %s.\n", msgSpecification);
        return;
    };
    if (!isdigit(address[0]) || !isdigit(address[address.length() - 1]))
    {
        Error::printError(IPV4_WRONG_FORMAT, "IPv4 address wrong format. Enter value among 0.0.0.0 and 255.255.255.255\n");
        return;
    }

    // function to parse correct ipv4
    size_t startPos = 0;
    while (startPos < address.npos)
    {
        size_t endPos = address.find(".", startPos);
        std::string ipPart = address.substr(startPos, endPos - startPos);

        char *garbage;
        long tokenValue = strtol(ipPart.c_str(), &garbage, 10);

        printf("%lu\n", tokenValue);
        if (tokenValue > 255 || tokenValue < 0)
        {
            Error::printError(IPV4_WRONG_FORMAT, "Invalid value\n");
            return;
        }

        if (strlen(garbage) > 0)
        {
            Error::printError(IPV4_WRONG_FORMAT, "IPv4 cannot contain any characters.\n");
            return;
        }

        if (endPos == address.npos)
        {
            return;
        }

        startPos = endPos + 1;
    }

    return;
}

/**
 * @brief checks IPv6 address format
 *
 * @param address
 * @return int
 */
void InputParser::checkIPv6AddressType(std::string address)
{
    if (address.length() > IPV6_MAX_LENGTH)
        return;

    return;
}

/**
 * @brief Choose address checking function based on type of query
 *
 * @param query
 * @return int
 */
void InputParser::checkAddressType(Query &query)
{
    if (query.getReversed())
    {
        QueryType qType = query.getType();
        for (auto addr : query.getAddressVector())
        {
            if (qType == A)
                checkIPv4AddressType(addr);
            else if (qType == AAAA)
                checkIPv6AddressType(addr);
        }
    }
}

/**
 * @brief Parses arguments and set program settings
 *
 * Iterates through every character of every argument. If it finds '-' searches for option and validates option argument if necessary.
 *
 */
void InputParser::parseArgs(Query &query)
{
    if (this->argc <= 1)
    {
        Error::printError(WRONG_ARGUMENTS, "Wrong number of arguments.\n");
        query.setIsOk(false);
        return;
    }
    setOptions(query);
    if (query.getServer() == "")
    {
        Error::printError(WRONG_ARGUMENTS, "Server has not been set.\n");
    }
    if (query.getAddressVector().empty())
    {
        Error::printError(WRONG_ARGUMENTS, "Address has not been set.\n");
    }
    checkAddressType(query);
}
