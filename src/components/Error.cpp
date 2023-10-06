#include <iostream>
#include <fstream>
#include "Error.h"

void printToDebugFile()
{
    std::ofstream debugFile;
    debugFile.open("example.txt");
    debugFile << "Writing this to a file.\n";
    debugFile.close();
}

void Error::printError()
{
#ifndef TEST
    std::cerr << "Error occured." << std::endl;
#endif
    return;
}
