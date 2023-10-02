#include <iostream>
#include "Query.h"

Query::Query()
{
    this->recursionDesired = false;
    this->reversed = false;
    this->type = A;
    this->port = -1;
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

void Query::printQueryOptions()
{
    printf("recursionDesired:   %d\n", this->recursionDesired);
    printf("reversed:           %d\n", this->reversed);
    printf("type:               %d\n", this->type);
    printf("port:               %d\n", this->port);
    printf("server:             %s\n", this->server.c_str());
    printf("address:            %s\n", this->address.c_str());
}
