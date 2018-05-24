
#include <string>
#include <list>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

TEST_CASE( "ReturnIntermediateAndFinalCollectionTypeVector", "IntermediateCollect" ) {
    int n = 100;
    std::vector<int> elems(n);
    std::vector<int> expectedIntermediateResult(n);
    std::vector<int> expectedFinalResult(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedIntermediateResult[i] = 3 * i;
        expectedFinalResult[i] = 3 * i + 2;
    };

    Collection<int, std::vector, Pipe> intermediateResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .map<int, int>( [](int *in) { *in *= 3; return in; } )
        .intermediateCollect<int, std::vector>();

    REQUIRE_THAT( intermediateResult.value(), Catch::Equals(expectedIntermediateResult) );

    std::vector<int> finalResult =
    	intermediateResult
        .stream()
        .map<int, int>( [](int *in) { *in += 2; return in; } )
        .collect<int, std::vector>();

    REQUIRE_THAT( finalResult, Catch::Equals(expectedFinalResult) );
}


TEST_CASE("ReturnIntermediateAndFinalCollectionTypeDeque", "IntermediateCollect") {
    int n = 100;
    std::vector<int> elems(n);
    std::deque<int> expectedIntermediateResult(n);
    std::deque<int> expectedFinalResult(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedIntermediateResult[i] = 2 * i;
        expectedFinalResult[i] = 2 * i * 3;
    };

    Collection<int, std::deque, Pipe> partialResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .map<int, int>( [](int *in) { *in *= 2; return in; } )
        .intermediateCollect<int, std::deque>();

    std::deque<int> intermediateResult = partialResult.value();
    for (unsigned int i = 0; i < expectedIntermediateResult.size(); i++) {
        REQUIRE( expectedIntermediateResult[i] == intermediateResult[i] );
    };

    std::deque<int> finalResult =
    	partialResult
        .stream()
        .map<int, int>( [](int *in) { *in *= 3; return in; } )
        .collect<int, std::deque>();

    for (unsigned int i = 0; i < expectedFinalResult.size(); i++) {
        REQUIRE( expectedFinalResult[i] == finalResult[i] );
    };
}


TEST_CASE("ReturnIntermediateAndFinalCollectionTypeList", "IntermediateCollect") {
    int n = 100;
    std::vector<int> elems(n);
    std::list<int> expectedIntermediateResult;
    std::list<int> expectedFinalResult;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedIntermediateResult.push_back( i + 1 );
        expectedFinalResult.push_back( (i + 1) * 3 );
    };

    Collection<int, std::list, Pipe> partialResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .map<int, int>( [](int *in) { *in += 1; return in; } )
        .intermediateCollect<int, std::list>();

    std::list<int> intermediateResult = partialResult.value();
    for (std::list<int>::iterator expectedIterator = expectedIntermediateResult.begin(), currentIterator = intermediateResult.begin(); 
         expectedIterator != expectedIntermediateResult.end(); 
         expectedIterator++, currentIterator++) {
    	REQUIRE( *expectedIterator == *currentIterator );
    }

    std::list<int> finalResult =
    	partialResult
        .stream()
        .map<int, int>( [](int *in) { *in *= 3; return in; } )
        .collect<int, std::list>();


    for (std::list<int>::iterator expectedIterator = expectedFinalResult.begin(), currentIterator = finalResult.begin(); 
         expectedIterator != expectedFinalResult.end(); 
         expectedIterator++, currentIterator++) {
        REQUIRE( *expectedIterator == *currentIterator );
    }
}


TEST_CASE("CollectIntermediateAndFinalElementsParallel", "IntermediateCollect") {
    int n = 10000;
    std::vector<int> elems(n);
    std::vector<int> expectedIntermediateResult(n);
    std::vector<int> expectedFinalResult(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        expectedIntermediateResult[i] = 3 * i;
        expectedFinalResult[i] = 3 * i + 2;
    };

    Collection<int, std::vector, Pipe> intermediateResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>( [](int *in) { *in *= 3; return in; } )
        .sort<int>()
        .stream()
        .intermediateCollect<int, std::vector>();

    REQUIRE_THAT( intermediateResult.value(), Catch::Equals(expectedIntermediateResult) );

    std::vector<int> finalResult =
    	intermediateResult
        .stream()
        .parallel(4)
        .map<int, int>( [](int *in) { *in += 2; return in; } )
        .sort<int>()
        .stream()
        .collect<int, std::vector>();

    REQUIRE_THAT( finalResult, Catch::Equals(expectedFinalResult) );
}

TEST_CASE("CollectIntermediateAndFinalObjects", "IntermediateCollect") {
    unsigned int noEmployees = 15;
    std::vector<Employee> elems;
    std::list<Employee> expectedIntermediateResult;
    std::list<Employee> expectedFinalResult;

    for (unsigned int i = 0; i <noEmployees; i++) {
        Employee employee(i + 20, "Employee" + ConvertNumberToString(i), 25000);
        elems.push_back(employee);
        expectedIntermediateResult.push_back(employee);

        if (i + 20 > 30) {
            Employee employee(i + 20, "Employee" + ConvertNumberToString(i), 25000);
            expectedFinalResult.push_back(employee);
        }
    };

    Collection<Employee, std::list, Pipe> partialResult =
        Pipe()
        .source<Employee>(elems.begin(), elems.end())
        .collect<Employee, std::list>();

    std::list<Employee> intermediateResult = partialResult.value();
    for (std::list<Employee>::iterator expectedIterator = expectedIntermediateResult.begin(), currentIterator = intermediateResult.begin();
         expectedIterator != expectedIntermediateResult.end(); 
         expectedIterator++, currentIterator++) {
        REQUIRE( expectedIterator->name == currentIterator->name );
    }

    std::list<Employee> finalResult =
    	partialResult
        .stream()
        .find<Employee>( [](Employee *e) { return e->age > 30; } )
        .collect<Employee, std::list>();

    for (std::list<Employee>::iterator expectedIterator = expectedFinalResult.begin(), currentIterator = finalResult.begin(); 
         expectedIterator != expectedFinalResult.end(); 
         expectedIterator++, currentIterator++) {
        REQUIRE( expectedIterator->name == currentIterator->name );
    }
}
