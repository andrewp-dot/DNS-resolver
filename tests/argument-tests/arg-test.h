#ifndef ARG_TEST_H
#define ARG_TEST_H

#define COLOR_RED "\033[0;31m"
#define COLOR_DEFAULT "\033[0m"

#define TEST_ASSERT(cond, msg)                                                                    \
    do                                                                                            \
    {                                                                                             \
        if (!(cond))                                                                              \
        {                                                                                         \
            std::cerr << COLOR_RED << "[Assertion error]: " << COLOR_DEFAULT << msg << std::endl; \
            return EXIT_FAILURE;                                                                  \
        }                                                                                         \
    } while (0)

/**
 * Test cases
 */
bool argTest1();

#endif
