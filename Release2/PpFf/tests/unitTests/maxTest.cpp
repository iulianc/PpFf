
#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

TEST_CASE( "MaxEmenetsCollection", "MaxOperator" ) {
    std::vector<int> elems = {7, 9, 5, 6, 3, 4, 9, 7, 8};
    int expectedResult = 9;

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .max<int>();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE( "MaxEmenetsCollectionParallel", "MaxOperator" ) {
    std::vector<int> elems = {7, 9, 5, 6, 3, 4, 9, 7, 8};
    int expectedResult = 9;

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(4)
        .max<int>();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE( "MaxNegativeEmenetsCollection", "MaxOperator" ) {
    std::vector<int> elems = {-7, -9, -5, -6, -3, -4, -7, -8, -9};
    int expectedResult = -3;

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .max<int>();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("Max_Elements_Collection_Parallel_Large_Number_of_elements", "MaxOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    int expectedResult = 10005;
    for ( int i = 0; i < n; i++ ) {
    	if(i == 6){
            elems[i] = 10005;
    	}
    	else{
            elems[i] = i;
    	}
    }

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(4)
        .max<int>();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("GetOlderEmployee", "MaxOperator") {
    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee((i + 1) % 3 == 0 ? i + 40 : i + 20,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    Employee expectedResult(48, "Employee8", 1000);

    Employee currentResult = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .max<Employee>( [](Employee *younger, Employee *e) { if(younger->age < e->age) *younger = *e;} );

    REQUIRE(currentResult.name == expectedResult.name);
    REQUIRE(currentResult.age == expectedResult.age);
}

TEST_CASE("GetOlderEMployeeParallel", "MaxOperator") {
    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee((i + 1) % 3 == 0 ? i + 40 : i + 20,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    Employee expectedResult(48, "Employee8", 1000);

    Employee currentResult = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .max<Employee>( [](Employee *younger, Employee *e) { if(younger->age < e->age) *younger = *e;} );

    REQUIRE(currentResult.name == expectedResult.name);
    REQUIRE(currentResult.age == expectedResult.age);
}
