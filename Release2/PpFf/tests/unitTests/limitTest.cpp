#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

TEST_CASE("Limit0", "LimitOperator") {
    int n = 1000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult = {};
    
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .limit<int>(0)
        .collect<int, std::vector>();
    
    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("LimitMiddle", "LimitOperator") {
    unsigned int n = 1000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult;
    
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        if (i < n / 2) expectedResult.push_back(i);
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .limit<int>(n/2)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("LimitN", "LimitOperator") {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult;
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedResult.push_back(i);
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .limit<int>(n)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("LimitMoreThanN", "LimitOperator") {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult;
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedResult.push_back(i);
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .limit<int>(n+1)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}
