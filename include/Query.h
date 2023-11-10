#ifndef QUERY_H
#define QUERY_H
#include <string>

enum QueryType
{
    A = 1,
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
    // std::vector<std::string> addressVector;
    bool isOk;

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
    void setIsOk(bool value) { this->isOk = value; };
    // void setAddressVector(std::vector<std::string> addrVector) { this->addressVector = addrVector; };
    // void pushAddress(std::string address) { this->addressVector.push_back(address); };

    // getters
    bool getRecursionDesired() const { return this->recursionDesired; };
    bool getReversed() const { return this->reversed; };
    bool getIPv6() const { return this->ipv6; };
    QueryType getType() const { return this->type; };
    port_t getPort() const { return this->port; };
    std::string getServer() const { return this->server; };
    std::string getAddress() const { return this->address; };
    // std::vector<std::string> getAddressVector() const { return this->addressVector; };
    bool getIsOk() const { return this->isOk; };

    /* supportive functions */
    void reverseStringVector(std::vector<std::string> &vec);
    void reverseIPv4();
    void reverseIPv6();

    std::string boolToString(bool expr);
    void printQueryOptions();
};

#endif
