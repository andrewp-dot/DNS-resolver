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
#include <string.h>
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
                    std::cerr << "Missing server." << std::endl;
                    query.setIsOk(false);
                    break;
                }
                if (isFlag(argv[i]))
                {
                    std::cerr << "Missing server." << std::endl;
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
                    std::cerr << "Undefined option: " << argv[i] << "" << std::endl;
                    query.setIsOk(false);
                    break;
                }
                if (isFlag(argv[i]))
                {
                    std::cerr << "Missing port." << std::endl;
                    query.setIsOk(false);
                    break;
                }
                if (isGoodPortFormat(argv[i]))
                {
                    query.setPort(atoi(argv[i]));
                }
                else
                {
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
            query.setAddress(argv[i]);
        }
    }
}

int InputParser::checkAddressType(Query &query)
{
    if (query.getReversed())
    {
        // return checkIp address
        return EXIT_SUCCESS;
    }
    else // return checkAddress or just success
        return EXIT_SUCCESS;
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
    // checkAddressType(query);
}
