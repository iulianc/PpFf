#include "catch.hpp"
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"

TEST_CASE("CollectionOfIntegers", "CountOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    pp::Pipe pipe;
    unsigned int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .count();

    REQUIRE(expectedResult == currentResult);
}


TEST_CASE("CollectionOfObjects", "CountOperator") {
    int n = 15;
    std::vector<Employee> elems;
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < n; i++) {
        Employee employee;
        employee.age = i + 1;
        employee.name = "Employee" + ConvertNumberToString(i);
        employee.salary = 25000;

        elems.push_back(employee);
    };

    pp::Pipe pipe;
    unsigned int currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .count();

    REQUIRE(expectedResult == currentResult);
}

TEST_CASE("CollectionOfPointerObjects", "CountOperator") {
    std::vector<Employee*> elems;
    unsigned int expectedResult = 15;

    for (unsigned int i = 0; i < expectedResult; i++) {
        Employee *employee = new Employee();
        employee->age = i + 1;
        employee->name = "Employee" + ConvertNumberToString(i);
        employee->salary = 25000;

        elems.push_back(employee);
    };

    pp::Pipe pipe;
    unsigned int currentResult = pipe
        .source<Employee*>(elems.begin(), elems.end())
        .count();

    REQUIRE(expectedResult == currentResult);
}


TEST_CASE("CountOnCollectionTypeDeque", "CountOperator") {
    int n = 2500;
    std::deque<Employee> elems;
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < n; i++) {
        Employee employee;
        employee.age = i + 1;
        employee.name = "Employee" + ConvertNumberToString(i);
        employee.salary = 130000;

        elems.push_back(employee);
    };

    pp::Pipe pipe;
    unsigned int currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .count();

    REQUIRE(expectedResult == currentResult);
}


TEST_CASE("CountParallel", "CountOperator") {
    int n = 1500;
    std::vector<Employee> elems(n);
    unsigned int expectedResult = n;

    for (unsigned int i = 0; i < elems.size(); i++) {
        Employee employee;
        employee.age = i + 1;
        employee.name = "Employee" + ConvertNumberToString(i);
        employee.salary = 45000;

        elems[i] = employee;
    };

    pp::Pipe pipe;
    unsigned int currentResult = pipe
        .source<Employee>(elems.begin(), elems.end())
        .parallel(4)
        .count();

    REQUIRE(expectedResult == currentResult);
}

