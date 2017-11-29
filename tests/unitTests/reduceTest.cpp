#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

int FuncReduce(int sum, int in) {
    return sum + in;
};

TEST_CASE( "SumCollectionOfInteger", "ReduceOperator" ) {
    int n = 1000;
    std::vector<int> elems(n);
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };
    int expectedResult = n * (n - 1) / 2;

    //typedef int (*reduceF)(int, int);

    pp::Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .reduce<int, int>(0, FuncReduce);
    
    REQUIRE(currentResult == expectedResult);
}


TEST_CASE( "AgerEmployee", "ReduceOperator" ) {
    int n = 100;
    std::vector<Employee> employees(n);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee( i == 2 ? i + 50 : i + 20,
                           "Employee" + ConvertNumberToString(i),
                           12000 );
        employees[i] = employee;
    };
    employees[78].age = 999;
    std::string expectedResult = "Employee78";

    //typedef Employee (*reduceF)(Employee, Employee);

    pp::Pipe pipe;
    Employee currentResult = pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduce<Employee, Employee>( [](Employee e1, Employee e2) ->Employee { return e1.age > e2.age ? e1 : e2; });

    REQUIRE(currentResult.name == expectedResult);
}


TEST_CASE( "TotalSalaryEmployees", "ReduceOperator" ) {
    int n = 1000;
    std::vector<Employee> employees(n);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee( i + 20, "Employee" + ConvertNumberToString(i), 1000 + (i * 20) );
        employees[i] = employee;
    };

    int expectedResult = n * 1000 + 20 * n * (n - 1) / 2;

    //	typedef int (*reduceF)(int, int);
    //	typedef int (*mapF)(Employee);

    pp::Pipe pipe;
    int currentResult = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel()
        .reduce<Employee, int>( 0, 
                                [](Employee e) ->int { return e.salary; } , 
                                [](int totalSalary, int salary) ->int { return totalSalary + salary; } );

    REQUIRE(currentResult == expectedResult);
}

