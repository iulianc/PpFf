
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;


TEST_CASE("CountElementsCollection", "StreamOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    unsigned int currentResult =
    	Pipe()
        .stream<int>(elems.begin(), elems.end())
        .parallel(4)
        .count();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("CountEmployeesCollection", "StreamOperator") {
    unsigned int n = 15;
    std::vector<Employee> elems;
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < n; i++) {
        Employee employee(i + 1, "Employee" + ConvertNumberToString(i), 25000);
        elems.push_back(employee);
    };

    Pipe pipe = Pipe()
    		.stream<Employee>(elems.begin(), elems.end());

    unsigned int currentResult =
        pipe
        .count();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("FilterEmployeesWithSalaryBiggerThan100", "StreamOperator") {
    std::vector<Employee> elems;
    std::vector<std::string> expectedResult(3);
    expectedResult = {"Employee3","Employee6","Employee9"};
    unsigned int noEmployees = 10;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i * 10,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    Pipe pipe = Pipe()
    		.stream<Employee>(elems.begin(), elems.end());

    std::vector<std::string> currentResult =
        pipe
        .find<Employee>( [](Employee *e) ->bool { return e->salary > 100; } )
        .map<Employee, std::string>( [](Employee *e) ->std::string* { return &(e->name); } )
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "GroupEmployeesCollection", "StreamOperator" ) {
    typedef std::vector<Employee> VALUE;
    typedef std::unordered_map<int, VALUE> CONTAINER;

    unsigned int noEmployees = 10;
    VALUE employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    employees[0].age = employees[1].age = employees[2].age = 22;
    employees[3].age = employees[4].age = 18;
    employees[5].age = employees[6].age = 55;
    employees[7].age = employees[8].age = 33;
    employees[9].age = 44;

    CONTAINER expectedResult =
        { {22, {employees[0], employees[1], employees[2]}},
          {18, {employees[3], employees[4]}},
          {55, {employees[5], employees[6]}},
          {33, {employees[7], employees[8]}},
          {44, {employees[9]}}
        };

    CONTAINER result =
        Pipe()
        .stream<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, Employee>([](Employee* e) { return &(e->age); });

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i].name == expectedResultValue[i].name);
        }
    }
}

TEST_CASE("GetOlderEmployeeParallelWithStream", "StreamOperator") {
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
        Pipe()
        .stream<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .max<Employee>( [](Employee *younger, Employee *e) { if(younger->age < e->age) *younger = *e;} );

    REQUIRE(currentResult.name == expectedResult.name);
    REQUIRE(currentResult.age == expectedResult.age);
}

TEST_CASE( "ReduceByAgeCountEmployeesWithStream", "StreamOperator" ) {
    typedef std::unordered_map<int, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    employees[0].age = employees[1].age = employees[2].age = 22;
    employees[3].age = employees[4].age = 18;
    employees[5].age = employees[6].age = 55;
    employees[7].age = employees[8].age = 33;
    employees[9].age = 44;

    CONTAINER expectedResult =
        { {18, 2 },
          {22, 3 },
          {33, 2 },
          {44, 1 },
          {55, 2 }
        };

    Reducer<Employee, int> reducer(0, [](int count, Employee _) { return count + 1; });

    Pipe pipe;

    CONTAINER result =
        Pipe()
        .stream<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, int, int>([](Employee *e) { return &(e->age); },
                                         reducer);

    std::map<int, int> sortedResult;
    for (auto it = result.begin(); it != result.end(); it++) {
    	sortedResult[it->first] = it->second;
    }

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = sortedResult[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}

TEST_CASE("CollectionOfDoubleElementsWithStream", "StreamOperator") {
	int n = 10000;
	std::vector<double> elems(n);
	double expectedResult = 0.1 * n * (n - 1) / 2.0;

	double temp = 0.0;
	for (unsigned int i = 0; i < elems.size(); i++) {
		elems[i] = temp;
		temp += 0.1;
	};

	double currentResult =
		Pipe()
		.stream<double>(elems.begin(), elems.end())
		.sum<double>();

	REQUIRE(currentResult == Approx(expectedResult));
}
