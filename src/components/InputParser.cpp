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
    this->query.type = A;
}

bool InputParser::verifyPort(char * port) {

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
void InputParser::parseArgs() {

    if(this->argc <= 1){
        std::cerr << "Wrong number of arguments" << std::endl; 
        return;
    }

    for(int i = 0; i < this->argc; i++)
    {
        int charIndex = 0;
        while (argv[charIndex] != '\0')
        {
            if(argv[i][charIndex] == '-')
            {
                charIndex += 1;
                switch (argv[i][charIndex])
                {
                case 'r':
                    this->query.recursionDesired = true;
                    break;
                case 'x':
                    this->query.reversed = true;
                    break;
                case '6':
                    this->query.type = AAAA;
                    break;

                //needs to be verified options
                case 's':
                    // verify address
                    break;
                case 'p':
                    //i += 1;
                    //bool isGoodPortFormat = verifyPort(argv[i]);
                    // if(isGoodPortFormat) { this->port = atoi(argv[i]); };
                    break;
                
                default:
                    std::cerr << "Undefined option." << std::endl;
                    break;
                }
            }
            
            charIndex += 1;
        }
    }
}


