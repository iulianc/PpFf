#include "catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"
#include "../../src/operators/Reducing.hpp"
#include <string>
#include <iostream>

using namespace PpFf;

void FuncReduce(int *sum, int *in) {
	*sum = *sum + *in;
};

TEST_CASE( "SumCollectionOfInteger", "ReduceOperator" ) {
    int n = 1000;
    std::vector<int> elems(n);
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };
    int expectedResult = n * (n - 1) / 2;

    Reducing< int, int > reducing(FuncReduce);

    Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .reduce<int, int>(reducing);

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

    Reducing<Employee, Employee> reducing([](Employee *e1, Employee *e2) ->void { if(e1->age < e2->age) *e1 = *e2; });

    Pipe pipe;
    Employee currentResult = pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduce<Employee, Employee>(reducing);

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

    Reducing<Employee, int> reducing( 0,
            						  [](int *totalSalary, Employee *e) ->void { *totalSalary += e->salary; } );

    Pipe pipe;
    int currentResult = pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduce<Employee, int>(reducing);

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE( "SumCollectionOfIntegerParallel", "ReduceOperator" ) {
    int n = 1000;
    std::vector<int> elems(n);
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };
    int expectedResult = n * (n - 1) / 2;

    Reducing<int, int> reducing( 0, FuncReduce, [](int *result, int *threadResult) -> void {*result += *threadResult;} );

    Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
		.parallel(4)
        .reduce<int, int>(reducing);

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE( "AgerEmployeeParallel", "ReduceOperator" ) {
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

    Reducing<Employee, Employee> reducing( [](Employee *e1, Employee *e2) ->void { if(e1->age < e2->age) *e1 = *e2; },
    									   [](Employee *e1, Employee *e2) ->void { if(e1->age < e2->age) *e1 = *e2; } );

    Pipe pipe;
    Employee currentResult = pipe
        .source<Employee>(employees.begin(), employees.end())
		.parallel(4)
        .reduce<Employee, Employee>(reducing);

    REQUIRE(currentResult.name == expectedResult);
}


TEST_CASE( "TotalSalaryEmployeesParallel", "ReduceOperator" ) {
    int n = 1000;
    std::vector<Employee> employees(n);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee( i + 20, "Employee" + ConvertNumberToString(i), 1000 + (i * 20) );
        employees[i] = employee;
    };

    int expectedResult = n * 1000 + 20 * n * (n - 1) / 2;

    Reducing<Employee, int> reducing( 0,
            						  [](int *totalSalary, Employee *e) ->void { *totalSalary += e->salary; },
									  [](int *result, int *threadResult) -> void {*result += *threadResult;} );

    Pipe pipe;
    int currentResult = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduce<Employee, int>(reducing);

    REQUIRE(currentResult == expectedResult);
}
