#include "gtest/gtest.h"
#include <string>
#include "../../src/pp/Pipe.hpp"

struct Employee {
	int id;
	std::string name;
	int salary;
};

bool FuncFind(int in){
	if(in % 2 == 0){
		return true;
	}

	return false;
};

TEST(FindOperator, FindEvenElementsUsingFunction) {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {0,2,4,6,8};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef bool (*findF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.find< int, findF >(FuncFind)
					.collect< int, std::vector >();

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}

TEST(FindOperator, FindOddElementsUsingLambdaFunction) {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {1,3,5,7,9};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef bool (*findF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.find< int, findF >(([](int in)->bool {if(in % 2 != 0) return true; return false;}))
					.collect< int, std::vector >();

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}

TEST(FindOperator, FilterEmployeeWithSalaryBiggerThanHundred) {
	std::vector< Employee > elems(10);
	std::vector< std::string > expectedResult(3);
	expectedResult = {"Employee3","Employee6","Employee9"};
	std::vector< std::string > currentResult;
	std::vector< Employee > temp;

	for(unsigned int i = 0; i < elems.size(); i++){
		Employee employee;
		employee.id = i + 1;
		employee.salary = i%3 == 0 ? i * 100 : i * 10;
		employee.name = "Employee" + std::to_string(i);
		elems[i] = employee;
	};

	typedef bool (*retrieveEmployeeWithSalaryBiggerThanHundred)(Employee);
	typedef std::string (*retrieveNameEmployee)(Employee);

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.find< Employee, retrieveEmployeeWithSalaryBiggerThanHundred >(([](Employee e)->bool {if(e.salary > 100) return true; return false;}))
					.map< Employee, std::string, retrieveNameEmployee >(([](Employee e)->std::string {return e.name;}))
					.collect< std::string, std::vector >();


	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}



