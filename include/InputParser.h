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

#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H
#include "Query.h"
#include <string>

#define FLAG_LENGTH 2

class InputParser
{
private:
    int argc;
    char **argv;

    bool isGoodPortFormat(const char *port);
    void setOptions(Query &query);
    void checkIPv4AddressType(std::string address);
    void checkIPv6AddressType(std::string address);
    void checkAddressType(Query &query);

public:
    InputParser(int &argc, char **argv);
    void parseArgs(Query &query);
};

#endif
