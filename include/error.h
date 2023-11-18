/**
 * @file Error.h
 * @author Adrián Ponechal (xponec01@stud.fit.vut.cz)
 * @brief Module that is used for reporting errors.
 * @date 2023-11-18
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef ERROR_H
#define ERROR_H

enum ErrorCode
{
    SUCCESS = 0,
    FORMAT_ERROR = 1,
    SERVER_FAILURE = 2,
    NAME_NOT_EXIST = 3,
    UNSUPPORTED_QUERY = 4,
    QUERY_REFUSED = 5,
    CONNECTION_FAILED = 6,
    WRONG_ARGUMENTS = 7,
    INTERNAL = 99
};

class Error
{
private:
    static ErrorCode errorCode;

public:
    static void
    printError(ErrorCode err, const char *format, ...);
    static ErrorCode getErrorCode();
    static void setErrorCode(ErrorCode err);
    static void setErrorByResponseCode(unsigned char errCode);
};

#endif
