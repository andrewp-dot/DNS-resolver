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
#include <string.h>
#include <ctype.h>
#include "InputParser.h"
#include "constants.h"

InputParser::InputParser(int &argc, char ** argv) {
    this->argc = argc;
    this->argv= argv;
}

bool InputParser::isGoodPortFormat(char * port) {

    for(unsigned long i = 0; i < strlen(port); i++) {
        if(!isalnum(port[i])) return false;
    }

    unsigned long portNum = strtoul(port, NULL,10);
    if( portNum < 0 || portNum > MAX_PORT_NUMBER) return false;

    return true;
}


/**
 * @brief Parses arguments and set program settings
 * 
 * Iterates through every character of every argument. If it finds '-' searches for option and validates option argument if necessary.
 *  
 */
void InputParser::parseArgs(Query &query) {

    if(this->argc <= 1){
        std::cerr << "Wrong number of arguments" << std::endl; 
        return;
    }

    // new function

    /**
     * TODO:
     * 1) add support for addres parsing -it has to be last arg
     * @param i 
     */
    for(int i = 0; i < this->argc-1; i++)
    {
        if(argv[i][0] == '-')
        {
            switch (argv[i][1]) {
            case 'r':
                query.setRecursionDesired(true);
                break;
            case 'x':
                query.setReversed(true);
                break;
            case '6':
                query.setType(AAAA);
                break;
            //needs to be verified options
            case 's':
                i += 1;
                query.setAddress(argv[i]);
                break;
            case 'p':
                i += 1;
                if(isGoodPortFormat(argv[i])) { 
                    query.setPort(atoi(argv[i])); 
                } else query.setIsOk(false);
                break;
            
            default:
                std::cerr << "Undefined option." << std::endl;
                return;
            }
        }   
    }

    //getAddr();
}

