#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

TEST_CASE("FindAnyElementExisteInCollection1", "AnyMachOperator") {
    std::vector<int> elems(10);
    bool expectedResult = true;

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .anyMach<int>([](int *in)->bool {return (*in == 3); } );

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("FindAnyElementExisteInCollection2", "AnyMachOperator") {
    std::vector<int> elems(10);
    bool expectedResult = false;

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .anyMach<int>([](int *in)->bool {return (*in == 11); } );

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("FindAnyElementExisteInCollection1Parallel", "AnyMachOperator") {
    std::vector<int> elems(10000);
    bool expectedResult = true;

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
		.parallel(4)
        .anyMach<int>([](int *in)->bool {return (*in == 3); } );

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("FindAnyElementExisteInCollection2Parallel", "AnyMachOperator") {
    std::vector<int> elems(10000);
    bool expectedResult = false;

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
		.parallel(4)
        .anyMach<int>([](int *in)->bool {return (*in == 10001); } );

    REQUIRE(currentResult == expectedResult);
}



TEST_CASE("FindAnyEmployeeWithAgeBetween30And40", "AnyMachOperator") {
    std::vector<Employee> elems;
    bool expectedResult = true;
    unsigned int noEmployees = 10;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i + 25,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    bool currentResult =
        Pipe()
        .source<Employee>(elems.begin(), elems.end())
        .anyMach<Employee>( [](Employee *e) ->bool { return (e->age > 30 && e->age < 40); } );

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("FindAnyEmployeeWithAgeBetween31And40", "AnyMachOperator") {
    std::vector<Employee> elems;
    bool expectedResult = false;
    unsigned int noEmployees = 10;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i * 10,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    bool currentResult =
        Pipe()
        .source<Employee>(elems.begin(), elems.end())
        .anyMach<Employee>( [](Employee *e) ->bool { return (e->age > 30 && e->age < 40); } );

    REQUIRE(currentResult == expectedResult);
}
