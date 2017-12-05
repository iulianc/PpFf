#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

int* FuncMap(int *in){
    *in = *in * 3;
    return in;
};


TEST_CASE( "UpdateElementsCollectionUsingFunction", "MapOperator" ) {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> expectedResult = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27};

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .map<int, int>(FuncMap)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE("UpdateElementsCollectionUsingLambdaFunction", "MapOperator") {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> expectedResult = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27};

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .map<int, int>(([](int *in){ *in = *in * 3; return in; }))
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE("RetriveObjectPropertyValue", "MapOperator") {
    unsigned int noEmployees = 10;
    std::vector<Employee> elems;
    std::vector<std::string> expectedResult(noEmployees);
    expectedResult = {"Employee0","Employee1","Employee2","Employee3","Employee4","Employee5","Employee6","Employee7","Employee8","Employee9"};

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i, "Employee" + ConvertNumberToString(i), 12000);
        elems.push_back(employee);
    };

    pp::Pipe pipe;
    std::vector<std::string> currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .map<Employee, std::string>(([](Employee *e)->std::string* { return &(e->name); }))
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}



TEST_CASE("UpdateElementsCollectionParallel", "MapOperator") {
    std::vector<int> elems = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> expectedResult = {0, 3, 6, 9, 12, 15, 18, 21, 24, 27};

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>(FuncMap)
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("UpdateElementsCollectionParallel Large number of elements", "MapOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);
    for ( int i = 0; i < n; i++ ) {
        elems[i] = i;
        expectedResult[i] = *(FuncMap(new int(i)));
    }

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>(FuncMap)
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}
