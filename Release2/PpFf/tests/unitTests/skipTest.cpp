#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

TEST_CASE("Skip0", "SkipOperator") {
    int n = 1000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult = {};
    
    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedResult.push_back(i);
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .skip<int>(0)
        .collect<int, std::vector>();
    
    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("SkipMiddle", "SkipOperator") {
    int n = 1000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult;
    
    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        if (i >= n / 2) expectedResult.push_back(i);
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .skip<int>(n/2)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("SkipN", "SkipOperator") {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult = {};
    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .skip<int>(n)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("SkipNMoins1", "SkipOperator") {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult = {n-1};
    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .skip<int>(n-1)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("SkipMoreThanN", "SkipOperator") {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult = {};
    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .skip<int>(n+1)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}
