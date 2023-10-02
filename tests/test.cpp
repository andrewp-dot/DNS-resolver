#include <iostream>
#include "./argument-tests/arg-test.h"
#include "../src/dns.h"

int allTests()
{
#ifdef TEST
    return test_main(0, NULL);

    // TEST_ASSERT(2 == 2, "Test failed.");
    // std::cout << "All test passed." << std::endl;
    // return EXIT_SUCCESS;
#endif
}

int main(void)
{
    return allTests();
}
