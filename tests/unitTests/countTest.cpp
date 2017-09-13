#include "catch.hpp"
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

TEST_CASE("CollectionOfIntegers", "CountOperator") {
	std::vector< int > elems(10);
	unsigned int expectedResult = 10;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source<int>(elems.begin(), elems.end())
					.count();

	REQUIRE(expectedResult == currentResult);
}


TEST_CASE("CollectionOfObjects", "CountOperator") {
	std::vector< Employee > elems;
	unsigned int expectedResult = 15;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < expectedResult; i++){
		Employee employee;
		employee.age = i + 1;
		employee.name = "Employee" + ConvertNumberToString(i);
		employee.salary = 25000;

		elems.push_back(employee);
	};

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.count();

	REQUIRE(expectedResult == currentResult);
}


TEST_CASE("CountOnCollectionTypeDeque", "CountOperator") {
	std::deque< Employee > elems;
	unsigned int expectedResult = 25;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < 25; i++){
		Employee employee;
		employee.age = i + 1;
		employee.name = "Employee" + i;
		employee.salary = 130000;

		elems.push_back(employee);
	};

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.count();

	REQUIRE(expectedResult == currentResult);
}


TEST_CASE("CountParallel", "CountOperator") {
	std::vector< Employee > elems(15);
	unsigned int expectedResult = 15;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		Employee employee;
		employee.age = i + 1;
		employee.name = "Employee" + i;
		employee.salary = 45000;

		elems[i] = employee;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.parallel(4)
					.count();

	REQUIRE(expectedResult == currentResult);
}

