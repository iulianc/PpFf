#include "catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"
#include "../../src/operators/Reducer.hpp"
#include <string>
#include <iostream>
#include <thread>

using namespace PpFf;

TEST_CASE( "SumCollectionOfInteger", "ReduceOperator" ) {
    int n = 1000;
    std::vector<int> elems(n);
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };
    int expectedResult = n * (n - 1) / 2;
    
    Reducer<int, int> reducer( [](int* sum, int* in) { *sum += *in; } );
    
    Pipe pipe;
    int currentResult = 
        pipe
        .source<int>(elems.begin(), elems.end())
        .reduce<int, int>(reducer);

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE( "SumCollectionOfInteger avec meme lambda comme accumulator et combiner", "ReduceOperator" ) {
    int n = 1000;
    std::vector<int> elems(n);
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };
    int expectedResult = n * (n - 1) / 2;
    
    Pipe pipe;
    int currentResult = 
        pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .reduce<int, int>([](int* sum, int* in) { *sum += *in; });

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

    Reducer<Employee, Employee> reducer([](Employee* e1, Employee* e2) { if (e1->age < e2->age) *e1 = *e2; });

    Pipe pipe;
    Employee currentResult = 
        pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduce<Employee, Employee>(reducer);

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

    Reducer<Employee, int> reducer( 0,
                                    [](int* totalSalary, Employee* e) { *totalSalary += e->salary; } );

    Pipe pipe;
    int currentResult = 
        pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduce<Employee, int>(reducer);

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE( "SumCollectionOfIntegerParallel", "ReduceOperator" ) {
    int n = 1000;
    std::vector<int> elems(n);
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };
    int expectedResult = n * (n - 1) / 2;

    Reducer<int, int> reducer( 0, 
                               [](int* sum, int* in) { *sum += *in; },
                               [](int* result, int* threadResult) { *result += *threadResult; } );
    

    int maxNbThreads =  std::thread::hardware_concurrency();

    // Test avec differents nombres de threads.
    SECTION( "Avec des puissances de 2" ) {
        for (int nbThreads = 1; nbThreads <= 2 * maxNbThreads; nbThreads *= 2) {
            Pipe pipe;
            int currentResult = 
                pipe
                .source<int>(elems.begin(), elems.end())
                .parallel(nbThreads)
                .reduce<int, int>(reducer);
        
            REQUIRE(currentResult == expectedResult);
        }
    }

    SECTION( "Avec des valeurs consecutives" ) {
        for (int nbThreads = 2; nbThreads <= maxNbThreads / 2; nbThreads += 1) {
            Pipe pipe;
            int currentResult = 
                pipe
                .source<int>(elems.begin(), elems.end())
                .parallel(nbThreads)
                .reduce<int, int>(reducer);
        
            REQUIRE(currentResult == expectedResult);
        }
    }
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

    auto employeeAgeMin = [](Employee* e1, Employee* e2) { if (e1->age < e2->age) *e1 = *e2; };

    Reducer<Employee, Employee> reducer( employeeAgeMin, employeeAgeMin );

    Pipe pipe;
    Employee currentResult = 
        pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduce<Employee, Employee>(reducer);

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

    Reducer<Employee, int> reducer( 0,
                                    [](int* totalSalary, Employee* e) { *totalSalary += e->salary; },
                                    [](int* result, int* threadResult) { *result += *threadResult; } );

    Pipe pipe;
    int currentResult = 
        pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduce<Employee, int>(reducer);

    REQUIRE(currentResult == expectedResult);
}
