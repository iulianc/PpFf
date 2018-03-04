
#include <string>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

TEST_CASE("PrintElementsCollection", "PeekOperator") {
    int n = 100;
    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);

    for(unsigned int i = 0; i < elems.size(); i++){
        elems[i] = i;
        expectedResult[i] = i;
    };

    std::vector<int> peekedResult;
    Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .peek<int>( [&peekedResult](int *in)->bool { peekedResult.push_back( *in ); } )
        .collect< int, std::vector >();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
    REQUIRE_THAT( peekedResult, Catch::Equals(expectedResult) );
}


TEST_CASE("PrintPropertyObject", "PeekOperator") {
    int n = 1000;
    std::vector<Employee> elems(n);
    std::vector< std::string > expectedResult(n);

    for(unsigned int i = 0; i < elems.size(); i++){
        Employee employee(i * 10,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems[i] = employee;
        expectedResult[i] = employee.name;
    };

    std::vector<std::string> peekedResult;
    Pipe pipe;
    std::vector<std::string> currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .peek<Employee>( [&peekedResult](Employee *e) { peekedResult.push_back(e->name); } )
        .map<Employee, std::string>( [](Employee *e) { return &(e->name); } )
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
    REQUIRE_THAT( peekedResult, Catch::Equals(expectedResult) );
}
