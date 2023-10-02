#ifndef ARG_TEST_H
#define ARG_TEST_H

#define COLOR_RED "\033[0;31m"
#define COLOR_DEFAULT "\033[0m"

#define TEST_ASSERT(cond, msg)                                       \
    if (!(cond))                                                     \
    {                                                                \
        std::cerr << COLOR_RED << msg << COLOR_DEFAULT << std::endl; \
        return EXIT_FAILURE;                                         \
    }

/**
 * Test cases
 */

#endif
