
#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

TEST_CASE( "objets1", "UML" ) {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .max<int>();

    REQUIRE( currentResult == 9 );
}

TEST_CASE( "objets2", "UML" ) {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(2)
        .max<int>();

    REQUIRE( currentResult == 9 );
}

TEST_CASE( "objets3", "UML" ) {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int expectedResult = 10;

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(2)
        .map<int, int>(([](int *in){ *in *= 3; return in; }))
        .parallel(4)
        .max<int>();

    REQUIRE( currentResult == 27 );
}

TEST_CASE( "objets4", "UML" ) {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int expectedResult = 10;

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(2)
        .map<int, int>(([](int *in){ *in *= 3; return in; }))
        .map<int, int>(([](int *in){ *in += 1; return in; }))
        .parallel(4)
        .max<int>();

    REQUIRE( currentResult == 28 );
}
