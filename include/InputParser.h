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

    /**
     * @brief checks if next program argument is a flag
     *
     * @return true
     * @return false
     */
    bool isFlag(const char *arg);

    /**
     * @brief Checks string port format
     *
     * @param port - port to be verified
     * @return true
     * @return false
     */
    bool isGoodPortFormat(const char *port);

    /**
     * @brief Set the Query Type based on query settings
     *
     * @param query
     */
    void setQueryType(Query &query);

    /**
     * @brief modifies query addresses based on query type
     *
     * @param query
     */
    void modifyAddress(Query &query);

    /**
     * @brief Set options of given query based on given arguments
     *
     * @param query
     */
    void setOptions(Query &query);

public:
    /**
     * @brief Construct a new Input Parser:: Input Parser object
     *
     * @param argc - number of program arguments
     * @param argv - program arguments array
     */
    InputParser(int &argc, char **argv);

    /**
     * @brief Parses arguments and set program settings
     *
     * Iterates through every character of every argument. If it finds '-' searches for option and validates option argument if necessary.
     *
     */
    void parseArgs(Query &query);
};

#endif
