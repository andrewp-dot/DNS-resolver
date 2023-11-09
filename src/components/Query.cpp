#include <iostream>
#include <string>
#include "Query.h"
#include "constants.h"

Query::Query()
{
    this->recursionDesired = false;
    this->reversed = false;
    this->ipv6 = false;
    this->type = A;
    this->port = DNS_PORT;
    this->server = "";
    this->isOk = true;
}

void Query::reverseStringVector(std::vector<std::string> &vec)
{
    size_t start = 0;
    size_t end = vec.size() - 1;

    for (size_t i = 0; i < end; i++)
    {
        std::string temp = vec[start];
        vec[start] = vec[end];
        vec[end] = temp;
        start += 1;
        end -= 1;
    }
}

void Query::reverseIPv4()
{
    size_t startPos = 0;

    std::vector<std::string> parsedAddress;
    std::string address = this->getAddressVector().at(0);
    while (startPos < address.npos)
    {
        // here is a mistake -> still finds same dot
        size_t endPos = address.find(".", startPos);
        std::string token = address.substr(startPos, endPos - startPos);

        parsedAddress.push_back(token);

        if (endPos >= address.npos)
        {
            break;
        }
        else
        {
            parsedAddress.push_back(".");
        }
        startPos = endPos + 1;
    }

    // reverse parsedAddress
    reverseStringVector(parsedAddress);

    // print parsed ipv4
    for (size_t i = 0; i < parsedAddress.size(); i++)
    {
        std::cout << parsedAddress.at(i);
    }

    std::cout << std::endl;
}

void Query::reverseIPv6()
{
    std::string parsedAddress;
    std::string address = this->getAddressVector().at(0);
    // count all 4 chars ->  translate this 4321:0:1:2:3:4:567:89ab
    // int this -> b.a.9.8.7.6.5.0.4.0.0.0.3.0.0.0.2.0.0.0.1.0.0.0.0.0.0.0.1.2.3.4
    // int this -> b.a.9.8.7.6.5.0.4.0.0.0.3.0.0.0.2.0.0.0.1.0.0.0.0.0.0.0.1.2.3.4.

    // idea -> going from behind -> place every 4 chars and again until end // MAX_CHARS
    for (size_t i = 0; i < address.size(); i++)
    {
        // push part
        // get part
        char part[IPV6_PART_SIZE] = {'0', '0', '0', '0'};
        for (size_t j = 0; j < IPV6_PART_SIZE; j++)
        {
            char currentChar = address[address.size() - i - j - 1];
            // if(isIPv6char(currentChar))
            if (currentChar == ':')
            {
                i += j;
                break;
            }
            part[j] = currentChar;
            if (j == IPV6_PART_SIZE - 1)
            {
                i += IPV6_PART_SIZE;
                break;
            }
        }
        // if (address[address.size() - i - 1] == ':')
        // {
        //     i += 1;
        // }
        for (size_t j = 0; j < IPV6_PART_SIZE; j++)
        {
            parsedAddress.push_back(part[j]);
            parsedAddress.append(".");
        }
        // parsedAddress.push_back(currentChar);
        // parsedAddress.append(".");
    }

    std::cout << parsedAddress << std::endl;
    // std::cout << address.size() << std::endl;
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
    // std::cout << "Address: " << this->address << std::endl;
}
