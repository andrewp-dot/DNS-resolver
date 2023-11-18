/**
 * @file Error.h
 * @author Adrián Ponechal (xponec01@stud.fit.vutbr.cz)
 * @login xponec01
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
    /**
     * @brief Prints error on the screen and sets error code
     *
     * @param err - code of error
     * @param format - format of the string, to be printed (like in printf)
     * @param ...
     */
    static void printError(ErrorCode err, const char *format, ...);

    /**
     * @brief Get the error code of the program
     *
     * @return ErrorCode
     */
    static ErrorCode getErrorCode();

    /**
     * @brief Set the first found error code
     *
     * @param err - code of error
     */
    static void setErrorCode(ErrorCode err);

    /**
     * @brief Set the error code got by answer from DNS server recieved in Message module
     *
     * @param errCode
     */
    static void setErrorByResponseCode(unsigned char errCode);
};

#endif
