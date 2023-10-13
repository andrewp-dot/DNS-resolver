#include <iostream>
#include "Query.h"
#include "constants.h"

Query::Query()
{
    this->recursionDesired = false;
    this->reversed = false;
    this->type = A;
    this->port = DNS_PORT;
    this->server = "";
    this->server = "";
    this->isOk = true;
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
