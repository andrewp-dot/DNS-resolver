/**
 * @file InputParser.h
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
 * @brief InputParser is component for argument parsing and generating query settings.
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INPUT_PARSER_H
#define INPUT_PARSER_H
#include "Query.h"
#include <string>
#include <vector>

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
