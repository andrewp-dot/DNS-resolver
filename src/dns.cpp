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
#include "InputParser.h"

#define USAGE "dns [-r] [-x] [-6] -s server [-p port] adresa"

int main(int argc, char ** argv ) {   
    InputParser parser(argc, argv);
    
    printf("Successfuly compiled.");
    
    return EXIT_SUCCESS;
}
