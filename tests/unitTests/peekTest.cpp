#include "gtest/gtest.h"
#include <string>
#include "../../src/pp/Pipe.hpp"

struct Employee {
	int id;
	std::string name;
	int salary;
	std::string toString(){return name;};
};

TEST(PeekOperator, PrintElementsCollection) {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {0,1,2,3,4,5,6,7,8,9};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef void (*printElemetsCollection)(int);

	std::cout << "Elements of collection: ";
	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.peek< int, printElemetsCollection >(([](int in) {std::cout << in << "; ";}))
					.collect< int, std::vector >();
	std::cout << "\n";

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}

TEST(PeekOperator, PrintPropertyObject) {
	std::vector< Employee > elems(10);
	std::vector< std::string > expectedResult(10);
	expectedResult = {"Employee0","Employee1","Employee2","Employee3","Employee4","Employee5","Employee6","Employee7","Employee8","Employee9"};
	std::vector< Employee > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		Employee employee;
		employee.id = i + 1;
		employee.salary = i%3 == 0 ? i * 100 : i * 10;
		employee.name = "Employee" + std::to_string(i);
		elems[i] = employee;
	};

	typedef void (*printNameEmployee)(Employee);

	std::cout << "Employees name: ";
	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.peek< Employee, printNameEmployee >(([](Employee e) {std::cout << e.name << "; ";}))
					.collect< Employee, std::vector >();
	std::cout << "\n";

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i].name);
	};
}
