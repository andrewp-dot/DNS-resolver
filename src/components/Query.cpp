#include <iostream>
#include "Query.h"

Query::Query()
{
    this->recursionDesired = false;
    this->reversed = false;
    this->type = A;
    this->port = 53;
    this->server = "";
    this->server = "";
    this->isOk = true;
}

void Query::setRecursionDesired(bool value)
{
    this->recursionDesired = value;
}

void Query::setReversed(bool value)
{
    this->reversed = value;
}

void Query::setType(queryType type)
{
    this->type = type;
}

void Query::setPort(port_t port)
{
    this->port = port;
}

void Query::setServer(std::string server)
{
    // strcpy(this->address, server);
    this->server = server;
}

void Query::setAddress(std::string address)
{
    this->address = address;
}

void Query::setIsOk(bool value)
{
    this->isOk = value;
}

// getters
bool Query::getRecursionDesired()
{
    return this->recursionDesired;
}

bool Query::getReversed()
{
    return this->reversed;
}

queryType Query::getType()
{
    return this->type;
}

port_t Query::getPort()
{
    return this->port;
}

std::string Query::getServer()
{
    return this->server;
}

std::string Query::getAddress()
{
    return this->address;
}

bool Query::getIsOk()
{
    return this->isOk;
}

std::string Query::boolToString(bool expr)
{
    if (expr)
        return "Yes";
    return "No";
}

void Query::printQueryOptions()
{
    std::cout << "Query type: " << (this->type == AAAA ? "AAAA" : "A") << ", ";
    std::cout << "Recursive: " << boolToString(this->recursionDesired) << ", ";
    std::cout << "Reversed query: " << boolToString(this->reversed) << std::endl;
    std::cout << "Server: " << this->server << std::endl;
    std::cout << "Address: " << this->address << std::endl;
}
