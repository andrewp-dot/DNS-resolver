#include <iostream>
#include "arg-test.h"

int main(void)
{
    TEST_ASSERT(2 == 2, "Test failed.");
    std::cout << "All test passed." << std::endl;
    return EXIT_SUCCESS;
}
