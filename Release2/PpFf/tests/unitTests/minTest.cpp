
#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

TEST_CASE( "MinEmenetsCollection", "MinOperator" ) {
    std::vector<int> elems = {7, 9, 5, 6, 3, 4, 7, 8, 9};
    int expectedResult = 3;

    Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .min<int>();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE( "MinNegativeEmenetsCollection", "MinOperator" ) {
    std::vector<int> elems = {7, 9, -5, -6, -3, -4, -7, 8, 9};
    int expectedResult = -7;

    Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .min<int>();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE( "MinEmenetsCollectionParallel", "MinOperator" ) {
    std::vector<int> elems = {7, 9, 5, 6, 3, 4, 7, 8, 9};
    int expectedResult = 3;

    Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
		.parallel(4)
        .min<int>();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("Min_Elements_Collection_Parallel_Large_Number_of_elements", "MinOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    int expectedResult = 6;
    for ( int i = 0; i < n; i++ ) {
    	if(i < 6){
    		elems[i] = i + 10;
    	}
    	else{
    		elems[i] = i;
    	}
    }

    Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .min<int>();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("GetYoungerEmployee", "MinOperator") {
	unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee((i + 1) % 3 == 0 ? i + 20 : i + 40,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

	Employee expectedResult(22, "Employee2", 1000);

	Pipe pipe;
	Employee currentResult = pipe
		.source<Employee>(employees.begin(), employees.end())
		.min<Employee>( [](Employee *younger, Employee *e) { if(younger->age > e->age) *younger = *e;} );

	REQUIRE(currentResult.name == expectedResult.name);
	REQUIRE(currentResult.age == expectedResult.age);
}

TEST_CASE("GetYoungerEMployeeParallel", "MinOperator") {
	unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee((i + 1) % 3 == 0 ? i + 20 : i + 40,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

	Employee expectedResult(22, "Employee2", 1000);

	Pipe pipe;
	Employee currentResult = pipe
		.source<Employee>(employees.begin(), employees.end())
		.parallel(4)
		.min<Employee>( [](Employee *younger, Employee *e) { if(younger->age > e->age) *younger = *e;} );

	REQUIRE(currentResult.name == expectedResult.name);
	REQUIRE(currentResult.age == expectedResult.age);
}
