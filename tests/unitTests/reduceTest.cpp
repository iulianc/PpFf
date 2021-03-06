#include "catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"
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

    Reducer<int, int> reducer(std::plus<int>{});

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .reduce<int, int>(reducer);

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE( "ProductCollectionOfInteger with no initial value", "ReduceOperator" ) {
    int n = 10;

    std::vector<int> elems(n);
    int expectedResult = 1;
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i+1;
        expectedResult *= i+1;
    };

    Reducer<int, int> reducer(std::multiplies<int>{});

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .reduce<int, int>(reducer);

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE( "ProductCollectionOfInteger with no initial value and no element", "ReduceOperator" ) {
    std::vector<int> elems(0);

    Reducer<int, int> reducer(std::multiplies<int>{});

    int currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .reduce<int, int>(reducer);

    REQUIRE(currentResult == 0);
}

 TEST_CASE( "SumCollectionOfInteger avec meme lambda comme accumulator et combiner", "ReduceOperator" ) {
     int n = 1000;
     std::vector<int> elems(n);
     for (unsigned int i = 0; i < elems.size(); i++) {
         elems[i] = i;
     };
     int expectedResult = n * (n - 1) / 2;

     int currentResult =
         Flow
         ::source<int>(elems.begin(), elems.end())
         .parallel(4)
         .reduce<int, int>(0, std::plus<int>{});

     REQUIRE(currentResult == expectedResult);
 }

TEST_CASE( "SumCollectionOfIntegerWithVariousThreads", "ReduceOperator" ) {
    int n = 1000;
    int initVal = 9; // Valeur arbitraire.
    
    std::vector<int> elems(n);
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i+1;
    };
    int expectedResult = n * (n + 1) / 2;

    Reducer<int, int> reducer(initVal, std::plus<int>{}, std::plus<int>{});

    SECTION( "Sans parallel") {
        int currentResult =
            Flow
            ::source<int>(elems.begin(), elems.end())
            .reduce<int, int>(reducer);
        
        REQUIRE(currentResult == initVal + expectedResult);
    }

    SECTION( "Avec parallel et divers nombres de threads" ) {
        int maxNbThreads =  std::thread::hardware_concurrency();
        for (int nbThreads = 1; nbThreads <= maxNbThreads; nbThreads *= 2) {
            int currentResult =
                Flow
                ::source<int>(elems.begin(), elems.end())
                .parallel(nbThreads)
                .reduce<int, int>(reducer);
            
            REQUIRE(currentResult == initVal * nbThreads + expectedResult);
        }
    }
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

    Reducer<Employee, Employee> reducer([](Employee e1, Employee e2) { return e1.age > e2.age ? e1 : e2; });

    Employee currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
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

    Reducer<Employee, int> reducer(0,
                                   [](int totalSalary, Employee e) { return totalSalary + e.salary; });

    int currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
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

    Reducer<int, int> reducer(0, std::plus<int>{},  std::plus<int>{});

    int maxNbThreads =  std::thread::hardware_concurrency();

    // Test avec differents nombres de threads.
    SECTION( "Avec des puissances de 2" ) {
        for (int nbThreads = 1; nbThreads <= 2 * maxNbThreads; nbThreads *= 2) {
            int currentResult =
                Flow
                ::source<int>(elems.begin(), elems.end())
                .parallel(nbThreads)
                .reduce<int, int>(reducer);

            REQUIRE(currentResult == expectedResult);
        }
    }

    SECTION( "Avec des valeurs consecutives" ) {
        for (int nbThreads = 2; nbThreads <= maxNbThreads / 2; nbThreads += 1) {
            int currentResult =
                Flow
                ::source<int>(elems.begin(), elems.end())
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

    auto employeeAgeMin = [](Employee e1, Employee e2) { return e1.age > e2.age ? e1 : e2; };

    Employee currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduce<Employee, Employee>(employees[0], employeeAgeMin);

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

    Reducer<Employee, int> reducer(0,
                                   [](int totalSalary, Employee e) { return totalSalary + e.salary; },
                                   std::plus<int>{});

    int currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduce<Employee, int>(reducer);

    REQUIRE(currentResult == expectedResult);
}
