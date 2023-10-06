#ifndef ERROR_H
#define ERROR_H

/**
 * This happens when the referenced name does not exist.  For
 * example, a user may have mistyped a host name.
 */
#define ERR_NAME 1

/**
 * This happens when the referenced name exists, but data of the
 * appropriate type does not.  For example, a host address
 * function applied to a mailbox name would return this error
 * since the name exists, but no address RR is present.
 */
#define ERR_NOT_FOUND 2
#define ERR_WRONG_ARGUMENTS 3
#define ERR_INTERNAL 99

class Error
{
public:
    static void printError();

    // private:
    //     int errorCode;
};

#endif
