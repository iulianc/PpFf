#include "catch.hpp"
#include <string>
#include <vector>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"


TEST_CASE( "GroupByKeyACollectionTypeVector", "GroupByKeyOperator" ) {
	typedef std::vector< std::string > VALUE;
	typedef std::map< std::string, VALUE > CONTAINER;

	std::vector< std::string > strElems(8);
	strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", "Employee9", "Employee3", "Employee3"};

	CONTAINER expectedResult;
	CONTAINER result;

	VALUE employees3 = {"Employee3", "Employee3", "Employee3", "Employee3"};
	VALUE employees6 = {"Employee6", "Employee6"};
	VALUE employees9 = {"Employee9", "Employee9"};

	expectedResult["Employee3"] = employees3;
	expectedResult["Employee6"] = employees6;
	expectedResult["Employee9"] = employees9;


	pp::Pipe pipe;
	result = pipe.source< std::string >(strElems.begin(), strElems.end())
						.groupByKey< std::string >();


	std::map<std::string, std::vector< std::string > >::iterator it = expectedResult.begin();
	while(it != expectedResult.end())
	{
		std::string expectedResultKey = it->first;

		VALUE expectedResultValue = it->second;
		VALUE resultValue = result[expectedResultKey];

		for(unsigned int i = 0; i < expectedResultValue.size(); i++){
			REQUIRE(expectedResultValue[i] == resultValue[i]);
		}

		it++;
	}
}

TEST_CASE( "GroupByAgeACollectionEmployees", "GroupByKeyOperator" ) {
	typedef std::vector< Employee > VALUE;
	typedef std::map< int, VALUE > CONTAINER;

	VALUE employees;
	CONTAINER expectedResult;
	CONTAINER result;

	unsigned int noEmployees = 10;

	for(unsigned int i = 0; i < noEmployees; i++){
		Employee employee;
		employee.salary = i%3 == 0 ? i * 100 : i * 10;
		employee.name = "Employee" + ConvertNumberToString(i);

		employees.push_back(employee);
	};

	employees[0].age = 22;
	employees[1].age = 22;
	employees[2].age = 22;
	employees[3].age = 18;
	employees[4].age = 18;
	employees[5].age = 55;
	employees[6].age = 55;
	employees[7].age = 33;
	employees[8].age = 33;
	employees[9].age = 44;

	expectedResult = {{22, {employees[0], employees[1], employees[2]}}, {18, {employees[3], employees[4]}}, {55, {employees[5], employees[6]}}, {33, {employees[7], employees[8]}}, {44, {employees[9]}}};


	typedef int (*myFunc)(Employee);

	pp::Pipe pipe;
	result = pipe.source< Employee >(employees.begin(), employees.end())
					.groupByKey< Employee, int, Employee, myFunc >(([](Employee e)->int {return e.age;}));


	std::map< int, std::vector< Employee > >::iterator it = expectedResult.begin();
	while(it != expectedResult.end())
	{
		int expectedResultKey = it->first;

		VALUE expectedResultValue = it->second;
		VALUE resultValue = result[expectedResultKey];

		for(unsigned int i = 0; i < expectedResultValue.size(); i++){
			REQUIRE(expectedResultValue[i].name == resultValue[i].name);
		}

		it++;
	}
}

TEST_CASE( "GroupByAgeAndCountEmployees", "GroupByKeyOperator" ) {
	typedef std::map< int, int > CONTAINER;

	std::vector< Employee > employees;
	CONTAINER expectedResult;
	CONTAINER result;

	unsigned int noEmployees = 10;

	for(unsigned int i = 0; i < noEmployees; i++){
		Employee employee;
		employee.salary = i%3 == 0 ? i * 100 : i * 10;
		employee.name = "Employee" + ConvertNumberToString(i);

		employees.push_back(employee);
	};

	employees[0].age = 22;
	employees[1].age = 22;
	employees[2].age = 22;
	employees[3].age = 18;
	employees[4].age = 18;
	employees[5].age = 55;
	employees[6].age = 55;
	employees[7].age = 55;
	employees[8].age = 33;
	employees[9].age = 44;

	expectedResult = {{22, 3}, {18, 2}, {55, 3}, {33, 1}, {44, 1}};

	typedef int (*myFunc)(Employee);
	typedef int (*biOp)(int, Employee);

	pp::Pipe pipe;
	result = pipe.source< Employee >(employees.begin(), employees.end())
				.groupByKey< Employee, int, int, myFunc, biOp >(([](Employee e)->int {return e.age;}), 0, ([](int count, Employee e)->int {return count+1;}));


	std::map< int, int >::iterator it = expectedResult.begin();
	while(it != expectedResult.end())
	{
		int expectedResultKey = it->first;

		int expectedResultValue = it->second;
		int resultValue = result[expectedResultKey];

		REQUIRE(expectedResultValue == resultValue);

		it++;
	}
}
