#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

TEST_CASE("PrintElementsCollection", "PeekOperator") {
    std::vector<int> elems(10);
    std::vector<int> expectedResult(10);
    
    for(unsigned int i = 0; i < elems.size(); i++){
        elems[i] = i;
        expectedResult[i] = i;
    };

    //typedef void (*printElemetsCollection)(int);

    std::cout << "Elements of collection: ";
    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .peek<int>( [](int in) { std::cout << in << "; "; } )
        .collect< int, std::vector >();
    std::cout << "\n";

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE("PrintPropertyObject", "PeekOperator") {
    std::vector<Employee> elems(10);
    std::vector< std::string > expectedResult(10);
    expectedResult = {"Employee0","Employee1","Employee2","Employee3","Employee4","Employee5","Employee6","Employee7","Employee8","Employee9"};


    for(unsigned int i = 0; i < elems.size(); i++){
        Employee employee;
        employee.age = i * 10;
        employee.salary = i%3 == 0 ? i * 100 : i * 10;
        employee.name = "Employee" + ConvertNumberToString(i);
        elems[i] = employee;
    };

    //typedef void (*printNameEmployee)(Employee);

    std::cout << "Name of Employees: ";
    pp::Pipe pipe;
    std::vector<Employee> currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .peek<Employee>( [](Employee e) { std::cout << e.name << "; "; } )
        .collect< Employee, std::vector >();
    std::cout << "\n";

    for(unsigned int i = 0; i < expectedResult.size(); i++){
        REQUIRE(expectedResult[i] == ((Employee)currentResult[i]).name);
    };
}
