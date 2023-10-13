#include <iostream>
// #include "./argument-tests/arg-test.h"
#include "../src/dns.h"
#include "test.h"
#include "Error.h"

int failedTests = 0;
int passedTests = 0;

void printResults()
{
    if (failedTests == 0)
    {
        std::cout << COLOR_GREEN << "All tests passed." << COLOR_DEFAULT << std::endl;
    }
    else
    {
        std::cout << COLOR_GREEN << "Passed tests: " << COLOR_DEFAULT << passedTests << " | ";
        std::cout << COLOR_RED << "Failed tests: " << COLOR_DEFAULT << failedTests << std::endl;
    }
}

void argTests()
{
#ifdef TEST
    // const char programName[] = "dns";
    // no arguments test
    UNIT_TEST(WRONG_ARGUMENTS, test_main(1, NULL), "No arguments test.");

    // clean error code...
    char *argv[] = {(char *)"dns", (char *)"-x", (char *)"0.0.0.0"};

    // good arguments tests
    int result = test_main(3, argv);
    UNIT_TEST(EXIT_SUCCESS, result, "reversed, 0.0.0.0");

    argv = {(char *)"dns", (char *)"-x", (char *)"0.0.0.0"};
    UNIT_TEST(EXIT_SUCCESS, result, "reversed, 0.0.0.0");

    // wrong arguments tests

#else
    std::cerr << "In order to run tests, define TEST in g++" << std::endl;
#endif
}

void allTests()
{
#ifdef TEST
    // int exitStatus = EXIT_SUCCESS;
    argTests();

    // TEST_ASSERT(2 == 2, "Test failed.");
    // std::cout << "All test passed." << std::endl;

    printResults();
#else
    std::cerr << "In order to run tests, define TEST in g++" << std::endl;
#endif
}

int main(void)
{
    allTests();
    return EXIT_SUCCESS;
}
