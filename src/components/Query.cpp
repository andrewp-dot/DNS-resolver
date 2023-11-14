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
    // std::string address = this->getAddressVector().at(0);
    std::string address = this->getAddress();
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
    parsedAddress.push_back(".");

    // convert vector to string
    std::string parsedAddressString;
    for (size_t i = 0; i < parsedAddress.size(); i++)
    {
        parsedAddressString.append(parsedAddress.at(i));
    }

    this->setAddress(parsedAddressString);
}

void Query::reverseIPv6()
{
    std::string parsedAddress;
    std::string address = this->getAddress();

    for (size_t i = 0; i < address.size(); i++)
    {
        // get part
        char part[IPV6_PART_SIZE] = {'0', '0', '0', '0'};
        for (size_t j = 0; j < IPV6_PART_SIZE; j++)
        {
            char currentChar = address[address.size() - i - j - 1];
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
        // push part
        for (size_t j = 0; j < IPV6_PART_SIZE; j++)
        {
            parsedAddress.push_back(part[j]);
            parsedAddress.append(".");
        }
    }
    this->setAddress(parsedAddress);
}

std::string Query::boolToString(bool expr)
{
    if (expr)
    {
        return "Yes";
    }
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
