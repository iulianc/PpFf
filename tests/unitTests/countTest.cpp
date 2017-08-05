#include "gtest/gtest.h"
#include "../../src/pp/Pipe.hpp"

TEST(CountOperator, CollectionOfIntegers) {
	std::vector< int > elems(10);
	unsigned int expectedResult = 10;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source<int>(elems.begin(), elems.end())
					.count();

    EXPECT_EQ(expectedResult, currentResult);
}

TEST(CountOperator, CollectionOfObjects) {
	struct Employee {
		int age;
		std::string name;
	};

	std::vector< Employee > elems(15);
	unsigned int expectedResult = 15;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		Employee employee;
		employee.age = i + 1;
		employee.name = "Employee" + i;

		elems[i] = employee;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.count();

    EXPECT_EQ(expectedResult, currentResult);
}


TEST(CountOperator, CollectionTypeDeque) {
	struct Employee {
		int age;
		std::string name;
	};

	std::deque< Employee > elems(25);
	unsigned int expectedResult = 25;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		Employee employee;
		employee.age = i + 1;
		employee.name = "Employee" + i;

		elems[i] = employee;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.count();

    EXPECT_EQ(expectedResult, currentResult);
}

TEST(CountOperator, Parallel) {
	struct Employee {
		int age;
		std::string name;
	};

	std::vector< Employee > elems(15);
	unsigned int expectedResult = 15;
	unsigned int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		Employee employee;
		employee.age = i + 1;
		employee.name = "Employee" + i;

		elems[i] = employee;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.parallel(4)
					.count();

    EXPECT_EQ(expectedResult, currentResult);
}
