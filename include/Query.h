/**
 * @file Query.h
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
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

public:
    Query();

    /**
     * @brief Set the recursionDesired flag
     *
     * @param value
     */
    void setRecursionDesired(bool value) { this->recursionDesired = value; };

    /**
     * @brief Set the reversed flag, if the query is reversed
     *
     * @param value
     */
    void setReversed(bool value) { this->reversed = value; };

    /**
     * @brief Set ipv6 flag. This flag indicates usage of IP version. If ipv6 is true, IPv6 addresses are used
     *
     * @param value
     */
    void setIPv6(bool value) { this->ipv6 = value; };

    /**
     * @brief Set the type of the query
     *
     * @param type
     */
    void setType(QueryType type) { this->type = type; };

    /**
     * @brief Set the port
     *
     * @param portNum
     */
    void setPort(port_t portNum) { this->port = portNum; };

    /**
     * @brief Set the server address
     *
     * @param server
     */
    void setServer(std::string server) { this->server = server; };

    /**
     * @brief Set the requested address
     *
     * @param address
     */
    void setAddress(std::string address) { this->address = address; };

    /**
     * @brief Get the recursionDesired flag value
     *
     * @return true
     * @return false
     */
    bool getRecursionDesired() const { return this->recursionDesired; };

    /**
     * @brief Get the reversed flag value
     *
     * @return true
     * @return false
     */
    bool getReversed() const { return this->reversed; };

    /**
     * @brief Get the ipv6 flag value
     *
     * @return true
     * @return false
     */
    bool getIPv6() const { return this->ipv6; };

    /**
     * @brief Get the type of the query object
     *
     * @return QueryType
     */
    QueryType getType() const { return this->type; };

    /**
     * @brief Get the used port of the query
     *
     * @return port_t
     */
    port_t getPort() const { return this->port; };

    /**
     * @brief Get the server address
     *
     * @return std::string
     */
    std::string getServer() const { return this->server; };

    /**
     * @brief Get the requested address
     *
     * @return std::string
     */
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
