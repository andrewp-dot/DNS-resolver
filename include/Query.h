#ifndef QUERY_H
#define QUERY_H
#include <string>

enum QueryType
{
    A,
    AAAA = 28
};

typedef unsigned int port_t;

class Query
{

private:
    bool recursionDesired;
    bool reversed;
    QueryType type;
    port_t port;
    std::string server;
    std::string address;
    bool isOk;

public:
    Query();

    // setters
    void setRecursionDesired(bool value) { this->recursionDesired = value; };
    void setReversed(bool value) { this->reversed = value; };
    void setType(QueryType type) { this->type = type; };
    void setPort(port_t portNum) { this->port = portNum; };
    void setServer(std::string server) { this->server = server; };
    void setAddress(std::string address) { this->address = address; };
    void setIsOk(bool value) { this->isOk = value; };

    // getters
    bool getRecursionDesired() const { return this->recursionDesired; };
    bool getReversed() const { return this->reversed; };
    QueryType getType() const { return this->type; };
    port_t getPort() const { return this->port; };
    std::string getServer() const { return this->server; };
    std::string getAddress() const { return this->address; };
    bool getIsOk() const { return this->isOk; };

    std::string boolToString(bool expr);
    void printQueryOptions();
};

#endif
