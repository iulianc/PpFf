#include "gtest/gtest.h"
#include <string>
#include "../../src/pp/Pipe.hpp"

int FuncMap(int in){
	return in * 3;
};

TEST(MapOperator, UpdateElementsCollectionUsingFunction) {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {0,3,6,9,12,15,18,21,24,27};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};
	
	typedef int (*mapF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.map< int, int, mapF >(FuncMap)
					.collect< int, std::vector >();
					
	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}

TEST(MapOperator, UpdateElementsCollectionUsingLambdaFunction) {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {0,3,6,9,12,15,18,21,24,27};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};
	
	typedef int (*mapF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.map< int, int, mapF >(([&](int in){return in * 3;}))
					.collect< int, std::vector >();
					
	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}

TEST(MapOperator, RetriveObjectPropertyValue) {
	struct Employee {
		int age;
		std::string name;
	};

	std::vector< Employee > elems(10);
	std::vector< std::string > expectedResult(10);
	expectedResult = {"Employee0","Employee1","Employee2","Employee3","Employee4","Employee5","Employee6","Employee7","Employee8","Employee9"};
	std::vector< std::string > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		Employee employee;
		employee.age = i;
		employee.name = "Employee" + std::to_string(i);
		elems[i] = employee;
	};
	
	typedef std::string (*retrieveNameEmployee)(Employee);

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.map< Employee, std::string, retrieveNameEmployee >(([&](Employee e){return e.name;}))
					.collect< std::string, std::vector >();
					
	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}

TEST(MapOperator, UpdateElementsCollectionParallel) {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {0,3,6,9,12,15,18,21,24,27};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef int (*mapF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.parallel(4)
					.map< int, int, mapF >(FuncMap)
					.collect< int, std::vector >();

	std::sort(currentResult.begin(), currentResult.end());
	for(unsigned int i = 0; i < expectedResult.size(); i++){
		EXPECT_EQ(expectedResult[i], currentResult[i]);
	};
}
