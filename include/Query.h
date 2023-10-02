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
public:
    Query();

    // setters
    void setRecursionDesired(bool value);
    void setReversed(bool value);
    void setType(queryType type);
    void setPort(port_t portNum);
    void setServer(std::string server);
    void setAddress(std::string address);
    void setIsOk(bool value);

    // getters
    bool getRecursionDesired();
    bool getReversed();
    queryType getType();
    port_t getPort();
    std::string getServer();
    std::string getAddress();
    bool getIsOk();

    void printQueryOptions();

private:
    bool recursionDesired;
    bool reversed;
    queryType type;
    port_t port;
    std::string server;
    std::string address;
    bool isOk;
};

#endif
