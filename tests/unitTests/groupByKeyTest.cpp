#include "catch.hpp"
#include <string>
#include <vector>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

TEST_CASE( "GroupByKeyACollectionTypeVector", "GroupByKeyOperator" ) {
    typedef std::vector<std::string> VALUE;
    typedef MapType<std::string, VALUE> CONTAINER;

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

TEST_CASE( "GroupByKeyACollectionTypeVectorGrandNombreElements", "GroupByKeyOperator" ) {
    typedef std::vector<std::string> VALUE;
    typedef MapType<std::string, VALUE> CONTAINER;

    int n = 200;
    CONTAINER expectedResult;
    std::vector<std::string> elems;
    for (int i = 0; i < n; i++) {
        std::vector<std::string> lesElems;
        for( int j = 0; j <= i; j++ ) {
            std::string en = "Employee" + ConvertNumberToString(i) ;
            elems.push_back(en);
            lesElems.push_back(en);
        }
        expectedResult["Employee" + ConvertNumberToString(i)] = lesElems;
    }

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(elems.begin(), elems.end())
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
    typedef MapType<int, VALUE> CONTAINER;

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

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, Employee>( [](Employee *e) ->int* { return new int(e->age); } );

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
    typedef MapType<int, int> CONTAINER;

    std::vector<Employee> employees;
    unsigned int NB = 1000;
    unsigned int noEmployees = 10 * NB;
    for (unsigned int i = 0; i < noEmployees; i++) {
        int age = 0;
        switch( i % 10 ) {
        case 0: case 1: case 2:
            age = 22; break;
        case 3: case 4:
            age = 18; break;
        case 5: case 6: case 7:
            age = 55; break;
        case 8:
            age = 33; break;
        case 9:
            age = 44; break;
        }
        Employee employee(age,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult = {{22, 3*NB}, {18, 2*NB}, {55, 3*NB}, {33, 1*NB}, {44, 1*NB}};


    pp::Pipe pipe;
    CONTAINER result = pipe
       .source<Employee>(employees.begin(), employees.end())
       .parallel(4)
       .groupByKey<Employee, int, int>( [](Employee *e) ->int* { return new int(e->age); },
                                        [](int &count, Employee *e) ->void { count = count + 1; }
                                        );

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}


