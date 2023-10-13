#ifndef TEST_H
#define TEST_H

#include "Error.h"

#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_DEFAULT "\033[0m"

extern int failedTests;
extern int passedTests;

#define UNIT_TEST(expected, got, msg)                                                             \
    do                                                                                            \
    {                                                                                             \
        if (!(expected == (int)got))                                                              \
        {                                                                                         \
            std::cerr << COLOR_RED << "[Assertion error]: " << COLOR_DEFAULT << msg << std::endl; \
            std::cerr << "Expected: " << expected << " | Got: " << (int)got << std::endl;         \
            failedTests += 1;                                                                     \
        }                                                                                         \
        else                                                                                      \
        {                                                                                         \
            passedTests += 1;                                                                     \
        }                                                                                         \
        Error::setErrorCode(SUCCESS);                                                             \
    } while (0)

void allTests();

#endif
