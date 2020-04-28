#include "catch.hpp"
#include "../../src/Flow.hpp"
#include <string>
#include <vector>
#include "Employee.hpp"
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
