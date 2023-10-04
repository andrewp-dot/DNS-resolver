#include <iostream>
#include <fstream>
#include "error.h"

void printToDebugFile()
{
    std::ofstream debugFile;
    debugFile.open("example.txt");
    debugFile << "Writing this to a file.\n";
    debugFile.close();
}

void printError()
{
    std::cerr << "Error occured." << std::endl;
}
