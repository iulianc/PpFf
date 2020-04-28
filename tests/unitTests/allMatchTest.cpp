#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

TEST_CASE("CheckAllElementsSatisfyCondition1", "AllMatchOperator") {
    int n = 1000;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .allMatch<int>([=](int *in) { return *in != (n+1); });

    REQUIRE(currentResult);
}

TEST_CASE("CheckAllElementsSatisfyCondition2", "AllMatchOperator") {
    int n = 100;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .allMatch<int>([](int *in) { return *in != 3; });

    REQUIRE(!currentResult);
}


TEST_CASE("CheckAllElemenstSatisfyCondition1Parallel", "AllMatchOperator") {
    int n = 100;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(4)
        .allMatch<int>([=](int *in) { return *in != (n+1); });
    
    REQUIRE(currentResult);
}

TEST_CASE("CheckAllElemensSatisfyCondition2Parallel", "AllMatchOperator") {
    int n = 10000;
    std::vector<int> elems(n);

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    bool currentResult =
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(4)
        .allMatch<int>([](int *in) { return *in == 3; });
    
    REQUIRE(!currentResult);
}



TEST_CASE("CheckAllEmployeesAgeBetween30And40", "AllMatchOperator") {
    std::vector<Employee> elems;
    unsigned int noEmployees = 10;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(33,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    bool currentResult =
        Flow
        ::source<Employee>(elems.begin(), elems.end())
        .allMatch<Employee>([](Employee *e) { return 30 <= e->age && e->age <= 40; });

    REQUIRE(currentResult);
}


TEST_CASE("CheckAllEmployeesWithAgeNotBetween30And40Bis", "AllMatchOperator") {
    std::vector<Employee> elems;
    unsigned int noEmployees = 10;

    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(i + 25,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        elems.push_back(employee);
    };

    bool currentResult =
        Flow
        ::source<Employee>(elems.begin(), elems.end())
        .allMatch<Employee>([](Employee *e) { return 30 <= e->age && e->age <= 40; });

    REQUIRE(!currentResult);
}
