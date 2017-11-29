#include "catch.hpp"
#include <string>
#include <vector>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

TEST_CASE( "GroupByKeyACollectionTypeVector", "GroupByKeyOperator" ) {
    typedef std::vector<std::string> VALUE;
    typedef std::map<std::string, VALUE> CONTAINER;
    
    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult;
    expectedResult["Employee3"] = {"Employee3", "Employee3", "Employee3", "Employee3"};
    expectedResult["Employee6"] = {"Employee6", "Employee6"};
    expectedResult["Employee9"] = {"Employee9", "Employee9"};

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(strElems.begin(), strElems.end())
        .groupByKey<std::string>();
    
    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        VALUE resultValue = result[it->first];
        VALUE expectedResultValue = it->second;

        REQUIRE_THAT(resultValue, Catch::Equals(expectedResultValue));
    }
}

TEST_CASE( "GroupByAgeACollectionEmployees", "GroupByKeyOperator" ) {
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
        { {22, {employees[0], employees[1], employees[2]}}, 
          {18, {employees[3], employees[4]}}, 
          {55, {employees[5], employees[6]}}, 
          {33, {employees[7], employees[8]}}, 
          {44, {employees[9]}} 
        };


    //typedef int (*myFunc)(Employee);

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, Employee>( [](Employee e) ->int { return e.age; } );

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

TEST_CASE( "GroupByAgeAndCountEmployees", "GroupByKeyOperator" ) {
    typedef std::map<int, int> CONTAINER;

    std::vector<Employee> employees;
    unsigned int noEmployees = 10;
    for (unsigned int i = 0; i < noEmployees; i++) {
        int age;
        if (i == 0 || i == 1 || i == 2) {
            age = 22;
        } else if ( i == 3 || i == 4 ) {
            age = 18;
        } else if ( i == 5 || i == 6 || i == 7 ) {
            age = 55;
        } else if ( i == 8 ) {
            age = 33;
        } else if ( i == 9 ) {
            age = 44;
        }  else {
            age = 0;
        }
        Employee employee(age,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult = {{22, 3}, {18, 2}, {55, 3}, {33, 1}, {44, 1}};

    //	typedef int (*myFunc)(Employee);
    //	typedef int (*biOp)(int, Employee);

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, int>( [](Employee e) ->int { return e.age; }, 
                                         0, 
                                         [](int count, Employee e) ->int { return count + 1; } 
                                         );

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}
