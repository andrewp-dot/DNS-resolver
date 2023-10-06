#ifndef QUERY_H
#define QUERY_H

enum queryType
{
    A,
    AAAA
};

typedef unsigned int port_t;

class Query
{

private:
    bool recursionDesired;
    bool reversed;
    queryType type;
    port_t port;
    std::string server;
    std::string address;
    bool isOk;

public:
    Query();

    // setters
    void setRecursionDesired(bool value) { this->recursionDesired = value; };
    void setReversed(bool value) { this->reversed = value; };
    void setType(queryType type) { this->type = type; };
    void setPort(port_t portNum) { this->port = portNum; };
    void setServer(std::string server) { this->server = server; };
    void setAddress(std::string address) { this->address = address; };
    void setIsOk(bool value) { this->isOk = value; };

    // getters
    bool getRecursionDesired() { return this->recursionDesired; };
    bool getReversed() { return this->reversed; };
    queryType getType() { return this->type; };
    port_t getPort() { return this->port; };
    std::string getServer() { return this->server; };
    std::string getAddress() { return this->address; };
    bool getIsOk() { return this->isOk; };

    std::string boolToString(bool expr);
    void printQueryOptions();
};

#endif
