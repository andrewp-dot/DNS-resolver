#include <iostream>
#include "./argument-tests/arg-test.h"

int allTests(void)
{
    TEST_ASSERT(2 == 2, "Test failed.");
    std::cout << "All test passed." << std::endl;
    return EXIT_SUCCESS;
}
