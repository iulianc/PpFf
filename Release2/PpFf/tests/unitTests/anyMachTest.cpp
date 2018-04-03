#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

TEST_CASE("FindAnyElementExisteInCollection1", "AnyMachOperator") {
    int n = 10;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .anyMach<int>([=](int *in) { return *in == 3; });

    REQUIRE(currentResult);
}

TEST_CASE("FindAnyElementExisteInCollection2", "AnyMachOperator") {
    int n = 100;
    std::vector<int> elems(n);

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .anyMach<int>([=](int *in) { return *in == (n+1); });

    REQUIRE(!currentResult);
}


TEST_CASE("FindAnyElementExisteInCollection1Parallel", "AnyMachOperator") {
    int n = 10000;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .anyMach<int>([](int *in) { return *in == 3; });

    REQUIRE(currentResult);
}

TEST_CASE("FindAnyElementExisteInCollection2Parallel", "AnyMachOperator") {
    int n = 10000;
    std::vector<int> elems(n);

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .anyMach<int>([=](int *in) { return *in == (n+1); });

    REQUIRE(!currentResult);
}



TEST_CASE("FindAnyEmployeeWithAgeBetween30And40", "AnyMachOperator") {
    unsigned int noEmployees = 10;
    std::vector<Employee> elems;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i + 25,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    bool currentResult =
        Pipe()
        .source<Employee>(elems.begin(), elems.end())
        .anyMach<Employee>([](Employee *e) { return e->age > 30 && e->age < 40; });

    REQUIRE(currentResult);
}


TEST_CASE("FindAnyEmployeeWithAgeBetween31And40", "AnyMachOperator") {
    std::vector<Employee> elems;
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
        .anyMach<Employee>([](Employee *e) { return e->age > 30 && e->age < 40; });

    REQUIRE(!currentResult);
}
