#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

int FuncReduce(int sum, int in){
	return sum += in;
};

TEST_CASE( "SumCollectionOfInteger", "ReduceOperator" ) {
	std::vector< int > elems(10);
	int expectedResult = 45;
	int currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef int (*reduceF)(int, int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.reduce< int, int, reduceF >(0, FuncReduce);

	REQUIRE(expectedResult == currentResult);
}


TEST_CASE( "AgerEmployee", "ReduceOperator" ) {
	std::vector< Employee > elemployees(10);
	std::string expectedResult = "Employee2";
	Employee currentResult;

	for(unsigned int i = 0; i < elemployees.size(); i++){
		Employee employee;

		if(i == 2){
			employee.age = i + 50;
		}else {
			employee.age = i + 20;
		}

		employee.name = "Employee" + ConvertNumberToString(i);
		employee.salary = 12000;
		elemployees[i] = employee;
	};

	typedef Employee (*reduceF)(Employee, Employee);

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elemployees.begin(), elemployees.end())
					.reduce< Employee, Employee, reduceF >(([](Employee e1, Employee e2)->Employee {return e1.age > e2.age ? e1 : e2;}));

	REQUIRE(expectedResult == currentResult.name);
}




TEST_CASE( "TotalSalaryEmployees", "ReduceOperator" ) {
	std::vector< Employee > elemployees(10);
	int expectedResult = 10900;
	int currentResult;

	for(unsigned int i = 0; i < elemployees.size(); i++){
		Employee employee;
		employee.age = i + 20;
		employee.name = "Employee" + ConvertNumberToString(i);
		employee.salary = 1000 + (i * 20);
		elemployees[i] = employee;
	};

	typedef int (*reduceF)(int, int);
	typedef int (*mapF)(Employee);

	pp::Pipe pipe;
	currentResult = pipe.source< Employee >(elemployees.begin(), elemployees.end())
					.reduce< Employee, int, reduceF, mapF >(0, ([](int totalSalary, int salary)->int {return totalSalary += salary;}), ([](Employee e)->int {return e.salary;}));

	REQUIRE(expectedResult == currentResult);
}
