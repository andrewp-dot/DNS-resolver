#include <iostream>
#include "arg-test.h"

int main(void)
{
    TEST_ASSERT(2 == 2, "1 != 2 you dumb ass bitch");
    std::cout << "No error has occured." << std::endl;
    return EXIT_SUCCESS;
}
