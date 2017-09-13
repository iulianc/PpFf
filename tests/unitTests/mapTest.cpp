#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

int FuncMap(int in){
	return in * 3;
};


TEST_CASE( "UpdateElementsCollectionUsingFunction", "MapOperator" ) {
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
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}


TEST_CASE("UpdateElementsCollectionUsingLambdaFunction", "MapOperator") {
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
					.map< int, int, mapF >(([](int in){return in * 3;}))
					.collect< int, std::vector >();

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}


TEST_CASE("RetriveObjectPropertyValue", "MapOperator") {
	std::vector< Employee > elems;
	std::vector< std::string > expectedResult(10);
	expectedResult = {"Employee0","Employee1","Employee2","Employee3","Employee4","Employee5","Employee6","Employee7","Employee8","Employee9"};
	std::vector< std::string > currentResult;
	unsigned int noEmployees = 10;

	for(unsigned int i = 0; i < noEmployees; i++){
		Employee employee;
		employee.age = i;
		employee.name = "Employee" + ConvertNumberToString(i);
		employee.salary = 12000;
		elems.push_back(employee);
	};

	typedef std::string (*retrieveNameEmployee)(Employee);

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elems.begin(), elems.end())
					.map< Employee, std::string, retrieveNameEmployee >(([](Employee e)->std::string {return e.name;}))
					.collect< std::string, std::vector >();

	for(unsigned int i = 0; i < currentResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}



TEST_CASE("UpdateElementsCollectionParallel", "MapOperator") {
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
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}
