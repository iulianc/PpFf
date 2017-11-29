
#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include <list>
#include "Employee.hpp"
#include "utility.hpp"

TEST_CASE( "ReturnCollectionTypeVector", "CollectOperator" ) {
    int n = 100;
    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedResult[i] = 3 * i;
    };

    //typedef int (*mapF)(int);

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .map<int, int>( [](int in) ->int { return 3 * in; } )
        .collect<int, std::vector>();
    
    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("ReturnCollectionTypeDeque", "CollectOperator") {
    int n = 100;
    std::vector<int> elems(n);
    std::deque<int> expectedResult(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedResult[i] = 2 * i;
    };

    //typedef int (*mapF)(int);

    pp::Pipe pipe;
    std::deque<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .map<int, int>( [](int in) ->int { return 2 * in; } )
        .collect<int, std::deque>();

    for (unsigned int i = 0; i < expectedResult.size(); i++) {
        REQUIRE(expectedResult[i] == currentResult[i]);
    };
}


TEST_CASE("ReturnCollectionTypeList", "CollectOperator") {
    int n = 100;
    std::vector<int> elems(n);
    std::list<int> expectedResult;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedResult.push_back( i + 1 );
    };

    //typedef int (*mapF)(int);

    pp::Pipe pipe;
    std::list<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .map<int, int>( [](int in) ->int { return in + 1; } )
        .collect<int, std::list>();

    std::list<int>::iterator currentIterator = currentResult.begin();
    for (std::list<int>::iterator expectedIterator = expectedResult.begin(); expectedIterator != expectedResult.end(); expectedIterator++) {
        REQUIRE(*expectedIterator == *currentIterator);
        currentIterator++;
    }
}


TEST_CASE("CollectElementsParallel", "CollectOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedResult[i] = 3 * i;
    };

    //typedef int (*mapF)(int);

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>( [](int in) ->int { return 3 * in; } )
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("CollectObjects", "CollectOperator") {
    unsigned int noEmployees = 15;
    std::vector<Employee> elems;
    std::list<Employee> expectedResult;

    for (unsigned int i = 0; i <noEmployees; i++) {
        Employee employee(i + 1, "Employee" + ConvertNumberToString(i), 25000);

        elems.push_back(employee);
        expectedResult.push_back(employee);
    };

    pp::Pipe pipe;
    std::list<Employee> currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .collect<Employee, std::list>();

    std::list<Employee>::iterator currentIterator = currentResult.begin();
    for (std::list<Employee>::iterator expectedIterator = expectedResult.begin(); expectedIterator != expectedResult.end(); expectedIterator++) {
        Employee expectedElem = *expectedIterator;
        Employee currentElem = *currentIterator;
        REQUIRE(expectedElem.name == currentElem.name);
        currentIterator++;
    }
}
