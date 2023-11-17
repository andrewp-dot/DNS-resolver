#ifndef ERROR_H
#define ERROR_H

enum ErrorCode
{
    SUCCESS = 0,
    FORMAT_ERROR = 1,
    SERVER_FAILURE = 2,
    /**
     * This happens when the referenced name does not exist.  For
     * example, a user may have mistyped a host name.
     */
    NAME_NOT_EXIST = 3,
    UNSUPPORTED_QUERY = 4,
    QUERY_REFUSED = 5,
    CONNECTION_FAILED,
    /**
     * This happens when the referenced name exists, but data of the
     * appropriate type does not.  For example, a host address
     * function applied to a mailbox name would return this error
     * since the name exists, but no address RR is present.
     */
    NOT_FOUND,
    WRONG_ARGUMENTS,
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
