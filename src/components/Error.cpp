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

void Error::setErrorByResponseCode(unsigned char errCode)
{
    switch (errCode)
    {
    case 0:
        setErrorCode(SUCCESS);
        return;
    case 1:
        Error::printError(FORMAT_ERROR, "The name server was unable to interpret query.\n");
        return;
    case 2:
        Error::printError(SERVER_FAILURE, "The name server was unable to process query.\n");
        return;
    case 3:
        Error::printError(NAME_NOT_EXIST, "The domain name refferenced in query does not exist.\n");
        return;
    case 4:
        Error::printError(UNSUPPORTED_QUERY, "The name server does not support requested query type.\n");
        return;
    case 5:
        Error::printError(QUERY_REFUSED, "The name server refuses to perform the specified operation for policy reasons.\n");
        return;
    default:
        return;
    }
}
