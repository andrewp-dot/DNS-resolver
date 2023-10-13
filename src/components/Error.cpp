#include <iostream>
#include <fstream>
#include <stdarg.h>
#include "Error.h"

// void printToDebugFile()
// {
//     std::ofstream debugFile;
//     debugFile.open("example.txt");
//     debugFile << "Writing this to a file.\n";
//     debugFile.close();
// }

ErrorCode Error::errorCode;

ErrorCode Error::getErrorCode()
{
    return Error::errorCode;
}

void Error::setErrorCode(ErrorCode err)
{
    if (errorCode == SUCCESS)
    {
        Error::errorCode = err;
    }
}

void Error::printError(ErrorCode err, const char *format, ...)
{
    Error::setErrorCode(err);
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);

    va_end(args);
    // exit(Error::getErrorCode());
    return;
}
