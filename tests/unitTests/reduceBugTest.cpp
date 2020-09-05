#include "catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"
#include "../../src/operators/Reducer.hpp"
#include <string>
#include <iostream>
#include <thread>

using namespace PpFf;

int n = 10;
int expectedResult = n * (n + 1) / 2;
std::vector<int> elems(n);

TEST_CASE( "AvecSection", "ReduceOperator" ) {
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i+1;
    };

    Reducer<int, int> reducer(0, std::plus<int>{}, std::plus<int>{});

    SECTION( "Une section") {
        std::cout << "Creation du flow" << std::endl; 
        int currentResult =
            Flow
            ::source<int>(elems.begin(), elems.end())
            .reduce<int, int>(reducer);
        
        std::cout << "Appel au REQUIRE" << std::endl; 
        REQUIRE(currentResult == expectedResult);
    }
}

TEST_CASE( "SansSectionSansReduceFinal", "ReduceOperator" ) {
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i+1;
    };

    Reducer<int, int> reducer(0, std::plus<int>{});

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .reduce<int, int>(reducer);
    
    REQUIRE(currentResult == expectedResult);
}

TEST_CASE( "SansSection", "ReduceOperator" ) {
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i+1;
    };

    Reducer<int, int> reducer(0, std::plus<int>{}, std::plus<int>{});

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .reduce<int, int>(reducer);
    
    REQUIRE(currentResult == expectedResult);
}
