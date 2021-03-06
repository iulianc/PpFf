
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

TEST_CASE("CollectionOfIntegers", "CountOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    unsigned int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(4)
        .count();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("CollectionOfObjects", "CountOperator") {
    unsigned int n = 15;
    std::vector<Employee> elems;
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < n; i++) {
        Employee employee(i + 1, "Employee" + ConvertNumberToString(i), 25000);
        elems.push_back(employee);
    };

    unsigned int currentResult = 
        Flow
        ::source<Employee>(elems.begin(), elems.end())
        .count();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("CollectionOfPointerObjects", "CountOperator") {
    std::vector<Employee*> elems;
    unsigned int expectedResult = 15;

    for (unsigned int i = 0; i < expectedResult; i++) {
        Employee *employee = new Employee(i + 1, "Employee" + ConvertNumberToString(i), 25000);
        elems.push_back(employee);
    };

    unsigned int currentResult = 
        Flow
        ::source<Employee*>(elems.begin(), elems.end())
        .count();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("CountOnCollectionTypeDeque", "CountOperator") {
    unsigned int n = 2500;
    std::deque<Employee> elems;
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < n; i++) {
        Employee employee(i + 1, "Employee" + ConvertNumberToString(i), 130000);
        elems.push_back(employee);
    };

    unsigned int currentResult = 
        Flow
        ::source<Employee>(elems.begin(), elems.end())
        .count();

    REQUIRE(currentResult == expectedResult);
}


TEST_CASE("CountParallel", "CountOperator") {
    int n = 1500;
    std::vector<Employee> elems(n);
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < elems.size(); i++) {
        Employee employee(i + 1, "Employee" + ConvertNumberToString(i), 45000);
        elems[i] = employee;
    };

    unsigned int currentResult = 
        Flow
        ::source<Employee>(elems.begin(), elems.end())
        .parallel(4)
        .count();

    REQUIRE(currentResult == expectedResult);
}
