
#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

TEST_CASE( "UML", "count1" ) {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int expectedResult = 10;

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .count();

    REQUIRE( currentResult == expectedResult );
}
