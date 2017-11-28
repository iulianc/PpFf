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
    int expectedResult = n * (n - 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    //typedef int (*reduceF)(int, int);

    pp::Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .reduce<int, int>(0, FuncReduce);
    
    REQUIRE(currentResult == expectedResult);
}


// A REGARDER (GUY T.)!?
TEST_CASE( "AgerEmployee", "ReduceOperator" ) {
    int n = 10;
    std::vector<Employee> elemployees(n);
    std::string expectedResult = "Employee2";

    for (unsigned int i = 0; i < elemployees.size(); i++) {
        Employee employee;
        if (i == 2) {
            employee.age = i + 50;
        } else {
            employee.age = i + 20;
        }

        employee.name = "Employee" + ConvertNumberToString(i);
        employee.salary = 12000;
        elemployees[i] = employee;
    };

    //typedef Employee (*reduceF)(Employee, Employee);

    pp::Pipe pipe;
    Employee currentResult = pipe
        .source<Employee>(elemployees.begin(), elemployees.end())
        .reduce<Employee, Employee>( [](Employee e1, Employee e2) ->Employee { return e1.age > e2.age ? e1 : e2; });

    REQUIRE(currentResult.name == expectedResult);
}


// A REGARDER (GUY T.)!?
TEST_CASE( "TotalSalaryEmployees", "ReduceOperator" ) {
    std::vector<Employee> elemployees(10);
    int expectedResult = 10900;

    for (unsigned int i = 0; i < elemployees.size(); i++) {
        Employee employee;
        employee.age = i + 20;
        employee.name = "Employee" + ConvertNumberToString(i);
        employee.salary = 1000 + (i * 20);
        elemployees[i] = employee;
    };

    //	typedef int (*reduceF)(int, int);
    //	typedef int (*mapF)(Employee);

    pp::Pipe pipe;
    int currentResult = pipe
        .source<Employee>(elemployees.begin(), elemployees.end())
        .reduce<Employee, int>( 0, 
                                [](Employee e) ->int { return e.salary; } , 
                                [](int totalSalary, int salary) ->int { return totalSalary + salary; } );

    REQUIRE(currentResult == expectedResult);
}

