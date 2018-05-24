#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

#include <iostream>

using namespace PpFf;

TEST_CASE( "SortElementsCollection", "SortOperator" ) {
    std::vector<int> elems = {2, 5, 0, 3, 9, 1, 6, 8, 7, 4};
    std::vector<int> expectedResult = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .sort<int>()
        .stream()
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "SortElementsCollectionParallel", "SortOperator" ) {
    std::vector<int> elems = {2, 5, 0, 3, 9, 1, 6, 8, 7, 4};
    std::vector<int> expectedResult = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .sort<int>()
        .stream()
    	.collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("SortElementsCollectionLargeNumberElements", "SortOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);
    for ( int i = 0; i < n; i++ ) {
    	elems[i] = i;
    	expectedResult[i] = i;
    }

    std::reverse(elems.begin(), elems.end());

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .sort<int>()
        .stream()
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE("SortEmployeesCollectionByAge", "SortOperator") {
    unsigned int noEmployees = 10;
    std::vector<Employee> elems(noEmployees);
    std::vector<Employee> expectedResult(noEmployees);

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i + 10, "Employee" + ConvertNumberToString(i), 12000);
        elems.push_back(employee);
        expectedResult.push_back(employee);
    };

    std::reverse(elems.begin(), elems.end());

    std::vector<Employee> currentResult =
        Pipe()
        .source<Employee>(elems.begin(), elems.end())
        .sort<Employee>( [](Employee e1, Employee e2) { return e1.age < e2.age; } )
        .stream()
    	.collect<Employee, std::vector>();

    for (unsigned int i = 0; i < noEmployees; i++) {
    	REQUIRE( currentResult[i].name == expectedResult[i].name );
    }
}

TEST_CASE("SortEmployeesCollectionByAgeParallel", "SortOperator") {
    unsigned int noEmployees = 10000;
    std::vector<Employee> elems(noEmployees);
    std::vector<Employee> expectedResult(noEmployees);

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i + 10, "Employee" + ConvertNumberToString(i), 12000);
        elems.push_back(employee);
        expectedResult.push_back(employee);
    };

    std::reverse(elems.begin(), elems.end());

    std::vector<Employee> currentResult =
        Pipe()
        .source<Employee>(elems.begin(), elems.end())
        .parallel(4)
        .sort<Employee>( [](Employee e1, Employee e2) { return e1.age < e2.age; } )
        .stream()
    	.collect<Employee, std::vector>();

    for (unsigned int i = 0; i < noEmployees; i++) {
    	REQUIRE( currentResult[i].name == expectedResult[i].name );
    }
}

