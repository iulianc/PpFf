#include "catch.hpp"
#include "../../src/Flow.hpp"
#include <string>
#include <vector>
#include "Employee.hpp"
#include "Student.hpp"
#include "utility.hpp"
#include <algorithm>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <iostream>

using namespace PpFf;


//unordered_map
TEST_CASE( "GroupByAgeACollectionEmployees", "GroupByKeyOperator" ) {
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
        Flow
        ::source<Employee>(employees.begin(), employees.end())
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



TEST_CASE( "GroupByAgeNamesEmployees", "GroupByKeyOperator" ) {
    typedef std::vector<Employee> EMPLOYEES;
    typedef std::vector<std::string> VALUE;
    typedef std::unordered_map<int, VALUE> CONTAINER;

    unsigned int noEmployees = 10;
    EMPLOYEES employees;
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
        { {22, {"Employee0", "Employee1", "Employee2"}},
          {18, {"Employee3", "Employee4"}},
          {55, {"Employee5", "Employee6"}},
          {33, {"Employee7", "Employee8"}},
          {44, {"Employee9"}}
        };

    CONTAINER result = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, std::string>([](Employee* e) { return &e->age; },
                                                [](Employee* e) { return &e->name; });

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i] == expectedResultValue[i]);
        }
    }
}


//map
TEST_CASE( "GroupByAgeACollectionEmployeesWithMap", "GroupByKeyOperator" ) {
    typedef std::vector<Employee> VALUE;
    typedef std::map<int, VALUE> CONTAINER;

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
        {
            {18, {employees[3], employees[4]}},
            {22, {employees[0], employees[1], employees[2]}},
            {33, {employees[7], employees[8]}},
            {44, {employees[9]}},
            {55, {employees[5], employees[6]}}
        };

    CONTAINER result = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, Employee, CONTAINER>([](Employee* e) { return &(e->age); });

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


//Parallel
//unordered_map
TEST_CASE( "GroupByAgeACollectionEmployeesParallel", "GroupByKeyOperator" ) {
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
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, int, Employee>([](Employee* e) { return &(e->age); });


    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        std::sort(resultValue.begin(), resultValue.end(), [](Employee e1, Employee e2)->bool {return e1.name < e2.name; } );

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i].name == expectedResultValue[i].name);
        }
    }
}


TEST_CASE( "GroupByAgeNamesEmployeesParallel", "GroupByKeyOperator" ) {
    typedef std::vector<Employee> EMPLOYEES;
    typedef std::vector<std::string> VALUE;
    typedef std::unordered_map<int, VALUE> CONTAINER;

    unsigned int noEmployees = 10;
    EMPLOYEES employees;
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
        { {22, {"Employee0", "Employee1", "Employee2"}},
          {18, {"Employee3", "Employee4"}},
          {55, {"Employee5", "Employee6"}},
          {33, {"Employee7", "Employee8"}},
          {44, {"Employee9"}}
        };

    CONTAINER result = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, int, std::string>([](Employee* e) { return &e->age; },
                                                [](Employee* e) { return &e->name; });

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        std::sort(resultValue.begin(), resultValue.end() );

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i] == expectedResultValue[i]);
        }
    }
}



//map
TEST_CASE( "GroupByAgeACollectionEmployeesWithMapParallel", "GroupByKeyOperator" ) {
    typedef std::vector<Employee> VALUE;
    typedef std::map<int, VALUE> CONTAINER;

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
        {
            {18, {employees[3], employees[4]}},
            {22, {employees[0], employees[1], employees[2]}},
            {33, {employees[7], employees[8]}},
            {44, {employees[9]}},
            {55, {employees[5], employees[6]}}
        };

    CONTAINER result = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, int, Employee, CONTAINER>([](Employee* e) { return &(e->age); });


    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        std::sort(resultValue.begin(), resultValue.end(), [](Employee e1, Employee e2)->bool {return e1.name < e2.name; } );

        REQUIRE(resultValue.size() == expectedResultValue.size());
        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(resultValue[i].name == expectedResultValue[i].name);
        }
    }
}



//unordered_map
TEST_CASE( "GroupByDepStudentsAverage", "GroupByKeyOperator" ) {
    typedef std::vector<Student> VALUE;
    typedef std::unordered_map<std::string, VALUE> CONTAINER;

    unsigned int noStudents = 10;
    VALUE students;
    for (unsigned int i = 0; i < noStudents; i++) {
        Student student("Student" + ConvertNumberToString(i),
                          "B",
                          "IT",
							"Montreal");
        students.push_back(student);
    };


    students[0].average = students[1].average = students[2].average = "A";
    students[3].average = students[4].average = "A";
    students[5].average = students[6].average = "B+";
    students[7].average = students[8].average = "A";
    students[9].average = "C";

    students[0].departement = students[1].departement = students[2].departement = "IT";
    students[3].departement = students[4].departement = "Accounting";
    students[5].departement = students[6].departement = "Chemistry";
    students[7].departement = students[8].departement = "Art";
    students[9].departement = "History";


    CONTAINER expectedResult =
        { {"IT", {students[0], students[1], students[2]}},
          {"Accounting", {students[3], students[4]}},
          {"Art", {students[7], students[8]}}
        };

    CONTAINER result = 
        Flow
        ::source<Student>(students.begin(), students.end())
			.find<Student>( [](Student *s) ->bool { return s->average == "A"; } )
        	.groupByKey<Student, std::string, Student>([](Student* s) { return &(s->departement); });

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


//unordered_map
TEST_CASE( "GroupByDepStudentsCity", "GroupByKeyOperator" ) {
    typedef std::vector<Student> VALUE;
    typedef std::unordered_map<std::string, VALUE> CONTAINER;

    unsigned int noStudents = 10;
    VALUE students;
    for (unsigned int i = 0; i < noStudents; i++) {
        Student student("Student" + ConvertNumberToString(i),
                          "B",
                          "IT",
							"Montreal");
        students.push_back(student);
    };


    students[0].average = students[1].average = students[2].average = "A";
    students[3].average = students[4].average = "A";
    students[5].average = students[6].average = "B+";
    students[7].average = students[8].average = "A";
    students[9].average = "C";

    students[0].departement = students[1].departement = students[2].departement = "IT";
    students[3].departement = students[4].departement = "Accounting";
    students[5].departement = students[6].departement = "Chemistry";
    students[7].departement = students[8].departement = "Art";
    students[9].departement = "History";

    students[0].city_address = students[1].city_address = students[2].city_address = "Montreal";
    students[3].city_address = students[4].city_address = "Quebec";
    students[5].city_address = students[6].city_address = "Longueuil";
    students[7].city_address = students[8].city_address = "Montreal";
    students[9].city_address = "Saskatchewan";


    CONTAINER expectedResult =
        { {"IT", {students[0], students[1], students[2]}},
          {"Art", {students[7], students[8]}}
        };

    CONTAINER result = 
        Flow
        ::source<Student>(students.begin(), students.end())
			.find<Student>( [](Student *s) ->bool { return s->average == "A" && s->city_address == "Montreal"; } )
        	.groupByKey<Student, std::string, Student>([](Student* s) { return &(s->departement); });

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


//unordered_map
TEST_CASE( "GroupByDepStudentsAverageParallel", "GroupByKeyOperator" ) {
    typedef std::vector<Student> VALUE;
    typedef std::unordered_map<std::string, VALUE> CONTAINER;

    unsigned int noStudents = 10;
    VALUE students;
    for (unsigned int i = 0; i < noStudents; i++) {
        Student student("Student" + ConvertNumberToString(i),
                          "B",
                          "IT",
							"Montreal");
        students.push_back(student);
    };


    students[0].average = students[1].average = students[2].average = "A";
    students[3].average = students[4].average = "A";
    students[5].average = students[6].average = "B+";
    students[7].average = students[8].average = "A";
    students[9].average = "C";

    students[0].departement = students[1].departement = students[2].departement = "IT";
    students[3].departement = students[4].departement = "Accounting";
    students[5].departement = students[6].departement = "Chemistry";
    students[7].departement = students[8].departement = "Art";
    students[9].departement = "History";


	CONTAINER expectedResult =
		{ 
			{"IT", {students[0], students[1], students[2]}},
			{"Accounting", {students[3], students[4]}},
			{"Art", {students[7], students[8]}}
		};

    CONTAINER result = 
        Flow
        ::source<Student>(students.begin(), students.end())
			.parallel(4)
			.find<Student>( [](Student *s) ->bool { return s->average == "A"; } )
        	.groupByKey<Student, std::string, Student>([](Student* s) { return &(s->departement); });

	


	REQUIRE(result.size() == expectedResult.size());
	for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
		VALUE resultValue = result[it->first];
		VALUE expectedResultValue = it->second;

		std::sort(resultValue.begin(), resultValue.end(), [](Student s1, Student s2)->bool {return s1.name < s2.name; } );

		REQUIRE(resultValue.size() == expectedResultValue.size());
		for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
			REQUIRE(resultValue[i].name == expectedResultValue[i].name);
		}
    }
}

