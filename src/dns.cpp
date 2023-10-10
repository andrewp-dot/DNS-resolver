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

#include <iostream>
#include <stdio.h>
#include "Error.h"
#include "InputParser.h"
#include "Message.h"

#define USAGE "dns [-r] [-x] [-6] -s server [-p port] adresa"

#ifdef TEST
#define MAIN test_main
#else
#define MAIN main
#endif

int MAIN(int argc, char **argv)
{
    InputParser parser(argc, argv);
    Query query;
    parser.parseArgs(query);
    if (Error::getErrorCode() == SUCCESS)
    {
        query.printQueryOptions();
    }

    // Message test = Message(query);
    // char buffer[512];
    // test.convertMsgToBuffer(buffer);
    return Error::getErrorCode();
}
