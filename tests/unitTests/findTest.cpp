#include "catch.hpp"
#include <string>
#include <vector>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

bool FuncFind(int in){
    return in % 2 == 0;
};


TEST_CASE("FindEvenElementsUsingFunction", "FindOperator") {
    std::vector<int> elems(10);
    std::vector<int> expectedResult = {0, 2, 4, 6, 8};

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    //typedef bool (*findF)(int);

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .find<int>(FuncFind)
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE("FindOddElementsUsingLambdaFunction", "FindOperator") {
    std::vector<int> elems(10);
    std::vector<int> expectedResult = {1, 3, 5, 7, 9};

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    //typedef bool (*findF)(int);

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .find<int>( [](int in) ->bool { return in % 2 != 0; } )
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE("FindOddElementsUsingLambdaFunction with large number of elements", "FindOperator") {
    int n = 100000;
    std::vector<int> elems(n);
    std::vector<int> expectedResult(n/2);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
        if (i % 2 == 0) {
            expectedResult[i/2] = i+1;
        }
    };

    //typedef bool (*findF)(int);

    pp::Pipe pipe;
    std::vector<int> currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(3)
        .find<int>( [](int in) ->bool { return in % 2 != 0; } )
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE("FilterEmployeeWithSalaryBiggerThanHundred", "FindOperator") {
    std::vector<Employee> elems;
    std::vector<std::string> expectedResult(3);
    expectedResult = {"Employee3","Employee6","Employee9"};
    unsigned int noEmployees = 10;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i * 10,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    //typedef bool (*retrieveEmployeesWithSalaryBiggerThanHundred)(Employee);
    //typedef std::string (*retrieveNameEmployee)(Employee);

    pp::Pipe pipe;
    std::vector<std::string> currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .find<Employee>( [](Employee e) ->bool { return e.salary > 100; } )
        .map<Employee, std::string>( [](Employee e) ->std::string { return e.name; } )
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}



