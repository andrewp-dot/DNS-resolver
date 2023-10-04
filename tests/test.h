#ifndef TEST_H
#define TEST_H

#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"
#define COLOR_DEFAULT "\033[0m"

extern int failedTests;
extern int passedTests;

#define TEST_ASSERT(cond, msg)                                                                    \
    do                                                                                            \
    {                                                                                             \
        if (!(cond))                                                                              \
        {                                                                                         \
            std::cerr << COLOR_RED << "[Assertion error]: " << COLOR_DEFAULT << msg << std::endl; \
            failedTests += 1;                                                                     \
        }                                                                                         \
        else                                                                                      \
            passedTests += 1;                                                                     \
    } while (0)

void allTests();

#endif
