/**
 * @file Query.h
 * @author Adrián Ponechal (xponec01@stud.fit.vut.cz)
 * @brief Query module stores user defined data next for processing.
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef QUERY_H
#define QUERY_H
#include <string>
#include <vector>

enum QueryType
{
    A = 1,
    NS = 2,
    CNAME = 5,
    SOA = 6,
    PTR = 12,
    AAAA = 28,
};

typedef unsigned int port_t;

class Query
{

private:
    bool recursionDesired;
    bool reversed;
    bool ipv6;
    QueryType type;
    port_t port;
    std::string server;
    std::string address;
    // bool isOk;

public:
    Query();

    // setters
    void setRecursionDesired(bool value) { this->recursionDesired = value; };
    void setReversed(bool value) { this->reversed = value; };
    void setIPv6(bool value) { this->ipv6 = value; };
    void setType(QueryType type) { this->type = type; };
    void setPort(port_t portNum) { this->port = portNum; };
    void setServer(std::string server) { this->server = server; };
    void setAddress(std::string address) { this->address = address; };

    // getters
    bool getRecursionDesired() const { return this->recursionDesired; };
    bool getReversed() const { return this->reversed; };
    bool getIPv6() const { return this->ipv6; };
    QueryType getType() const { return this->type; };
    port_t getPort() const { return this->port; };
    std::string getServer() const { return this->server; };
    std::string getAddress() const { return this->address; };

    /**
     * @brief Reverses
     *
     * @param vec
     */
    void reverseStringVector(std::vector<std::string> &vec);

    /**
     * @brief Reverses IPv4
     *
     */
    void reverseIPv4();

    /**
     * @brief Reverses IPv6
     *
     */
    void reverseIPv6();

    /**
     * @brief Converts boolean to string
     *
     * @param expr
     * @return std::string - "Yes" if expr is true, "No" if expr is false
     */
    std::string boolToString(bool expr);

    /**
     * @brief Prints options of the query
     *
     */
    void printQueryOptions();
};

#endif
