#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

TEST_CASE("CheckAnyElementExisteInCollection1", "NoneMachOperator") {
    int n = 100;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .noneMatch<int>([=](int *in)->bool { return *in == (n+1); });

    REQUIRE(currentResult);
}

TEST_CASE("CheckAnyElementExisteInCollection2", "NoneMachOperator") {
    int n = 10000;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .noneMatch<int>([](int *in) { return *in == 3; });

    REQUIRE(!currentResult);
}


TEST_CASE("CheckAnyElementExisteInCollection1Parallel", "NoneMachOperator") {
    std::vector<int> elems(10000);
    bool expectedResult = true;

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
		.parallel(4)
        .noneMatch<int>([](int *in)->bool {return (*in == 10001); } );

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("CheckAnyElementExisteInCollection2Parallel", "NoneMachOperator") {
    int n = 10000;
    std::vector<int> elems(n);

    for(unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
		.parallel(4)
        .noneMatch<int>([](int *in) { return *in == 3; });

    REQUIRE(!currentResult);
}


TEST_CASE("CheckAnyEmployeeWithAgeBetween30And40", "NoneMachOperator") {
    unsigned int noEmployees = 10;
    std::vector<Employee> elems;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i * 10,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    bool currentResult =
        Flow
        ::source<Employee>(elems.begin(), elems.end())
        .noneMatch<Employee>([](Employee *e) ->bool { return e->age > 30 && e->age < 40; });

    REQUIRE(currentResult);
}


TEST_CASE("CheckAnyEmployeeWithAgeBetween31And40", "NoneMachOperator") {
    unsigned int noEmployees = 100;
    std::vector<Employee> elems;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i + 25,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    bool currentResult =
        Flow
        ::source<Employee>(elems.begin(), elems.end())
        .noneMatch<Employee>([](Employee *e) { return e->age > 30 && e->age < 40; });

    REQUIRE(!currentResult);
}
