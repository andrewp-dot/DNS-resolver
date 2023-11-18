/**
 * @brief  Dns resolver for course ISA on VUT BUT Brno. It resolves address of specified server.
 * Copyright (C) 2023  xponec01@stud.fit.vutbr.cz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file dns.cpp
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
 * @brief Main body of dns resolver program.
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <stdio.h>
#include "Error.h"
#include "InputParser.h"
#include "Message.h"
#include "Connection.h"

int main(int argc, char **argv)
{
    InputParser parser(argc, argv);
    Query query;
    parser.parseArgs(query);

    if (Error::getErrorCode() == SUCCESS)
    {
        Connection connection = Connection();
        connection.sendUdpQuery(query);
    }
    return Error::getErrorCode();
}
