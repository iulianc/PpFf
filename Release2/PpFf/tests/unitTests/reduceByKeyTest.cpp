#include "catch.hpp"
#include "../../src/Pipe.hpp"
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

int plus1(int count, std::string _) {
    return count + 1;
};

TEST_CASE( "ReduceByKeyCountElementsContainer", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult = {{"Employee9", 2}, {"Employee6", 3}, {"Employee3", 4}};

    Reducer< std::string, int > reducer(0, plus1);

    CONTAINER result = 
        Pipe()
        .source<std::string>(strElems.begin(), strElems.end())
        .reduceByKey<std::string, std::string, int>(reducer);

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}


TEST_CASE( "ReduceInOrderByKeyCountElementsContainer", "ReduceByKeyOperator" ) {
    typedef std::map<std::string, int> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult = {{"Employee9", 2}, {"Employee6", 3}, {"Employee3", 4}};

    Reducer<std::string, int> reducer(0, plus1);

    CONTAINER result =
        Pipe()
        .source<std::string>(strElems.begin(), strElems.end())
        .reduceByKey<std::string, std::string, int, std::map<std::string, int>>(reducer);


    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}


TEST_CASE( "ReduceByAgeCountEmployees", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<int, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
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
        { {18, 2 },
          {22, 3 },
          {33, 2 },
          {44, 1 },
          {55, 2 }
        };

    Reducer<Employee, int> reducer(0, [](int count, Employee _) { return count + 1; });

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, int, int>(reducer, [](Employee *e) { return &(e->age); });

    std::map<int, int> sortedResult;
    for (auto it = result.begin(); it != result.end(); it++) {
    	sortedResult[it->first] = it->second;
    }

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = sortedResult[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}


TEST_CASE( "ReduceByJobTitleEmployeesSumSalary", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10,
                          i % 3 == 0 ? "manager" : "technician");
        employees.push_back(employee);
    };

    CONTAINER expectedResult =
        { {"manager", 1800},
          {"technician",  270 }
        };

    Reducer<Employee, int> reducer(0, 
                                   [](int totalSalary, Employee e) { return totalSalary + e.salary; });

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, std::string, int>(reducer, [](Employee *e) { return &(e->job_title); });


    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}




TEST_CASE( "ReduceByJobTitleEmployeesMaxAge", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10,
                          i % 3 == 0 ? "manager" : "technician");
        employees.push_back(employee);
    };

    employees[0].age = 40;
    employees[1].age = employees[2].age = 23;
    employees[3].age = 30;
    employees[4].age = employees[5].age = 18;
    employees[6].age = 40;
    employees[7].age = employees[8].age = 30;
    employees[9].age = 50;

    CONTAINER expectedResult =
        { {"manager", 50 },
          {"technician",  30 }
        };

    Reducer<Employee, int> reducer(0, 
                                   [](int maxAge, Employee e) { return maxAge < e.age ? e.age : maxAge; });

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, std::string, int>(reducer, [](Employee *e) { return &(e->job_title); });


    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}


TEST_CASE( "ReduceByJobTitleEmployeesMinAge", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10,
                          i % 3 == 0 ? "manager" : "technician");
        employees.push_back(employee);
    };

    employees[0].age = 40;
    employees[1].age = employees[2].age = 23;
    employees[3].age = 30;
    employees[4].age = employees[5].age = 18;
    employees[6].age = 40;
    employees[7].age = employees[8].age = 30;
    employees[9].age = 50;

    CONTAINER expectedResult =
        { {"manager", 30 },
          {"technician",  18 }
        };

    Reducer<Employee, int> reducer(1000, 
                                   [](int minAge, Employee e) { return minAge > e.age ? e.age : minAge; });

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, std::string, int>(reducer, [](Employee *e) { return &(e->job_title); });


    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}

//parallel
TEST_CASE( "ReduceByKeyCountElementsContainerParallel", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult = {{"Employee3", 4}, {"Employee6", 3}, {"Employee9", 2}};

    Reducer<std::string, int> reducer(0,
                                      plus1, 
                                      std::plus<int>());

    CONTAINER result = 
        Pipe()
        .source<std::string>(strElems.begin(), strElems.end())
        .parallel(4)
        .reduceByKey<std::string, std::string, int>(reducer);


    std::map<std::string, int> sortedResult;
    for (auto it = result.begin(); it != result.end(); it++) {
        sortedResult[it->first] = it->second;
    }

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = sortedResult[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}

TEST_CASE( "ReduceByAgeCountEmployeesParallel", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<int, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
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
        { {18, 2 },
          {22, 3 },
          {33, 2 },
          {44, 1 },
          {55, 2 }
        };

    Reducer< Employee, int > reducer(0,
                                     [](int count, Employee e) { return count + 1; }, 
                                     [](int total, int workerResult) { return total + workerResult; });

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, int, int>(reducer, [](Employee *e) { return &(e->age); });

    std::map<int, int> sortedResult;
    for (auto it = result.begin(); it != result.end(); it++) {
    	sortedResult[it->first] = it->second;
    }

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = sortedResult[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}


TEST_CASE( "ReduceByJobTitleEmployeesSumSalaryParallel", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10,
                          i % 3 == 0 ? "manager" : "technician");
        employees.push_back(employee);
    };

    CONTAINER expectedResult =
        { {"manager", 1800},
          {"technician",  270 }
        };

    Reducer< Employee, int > reducer(0, 
                                     [](int salary, Employee e) { return salary + e.salary; },
                                     std::plus<int>());

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, std::string, int>(reducer, [](Employee *e) { return &(e->job_title); });

    std::map<std::string, int> sortedResult;
    for (auto it = result.begin(); it != result.end(); it++) {
    	sortedResult[it->first] = it->second;
    }

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = sortedResult[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}



TEST_CASE( "ReduceByJobTitleEmployeesMaxAgeParallel", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10,
                          i % 3 == 0 ? "manager" : "technician");
        employees.push_back(employee);
    };

    employees[0].age = 40;
    employees[1].age = employees[2].age = 23;
    employees[3].age = 30;
    employees[4].age = employees[5].age = 18;
    employees[6].age = 40;
    employees[7].age = employees[8].age = 30;
    employees[9].age = 50;

    CONTAINER expectedResult =
        { {"manager", 50 },
          {"technician",  30 }
        };

    Reducer<Employee, int> reducer(0, 
                                   [](int maxAge, Employee e) { return maxAge < e.age ? e.age : maxAge; },
                                   [](int max, int workerResult) { return max < workerResult ? workerResult : max; } );

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, std::string, int>(reducer, [](Employee *e) { return &(e->job_title); });

    std::map<std::string, int> sortedResult;
    for (auto it = result.begin(); it != result.end(); it++) {
    	sortedResult[it->first] = it->second;
    }

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = sortedResult[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}


TEST_CASE( "ReduceByJobTitleEmployeesMinAgeParallel", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(0,
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10,
                          i % 3 == 0 ? "manager" : "technician");
        employees.push_back(employee);
    };

    employees[0].age = 40;
    employees[1].age = employees[2].age = 23;
    employees[3].age = 30;
    employees[4].age = employees[5].age = 18;
    employees[6].age = 40;
    employees[7].age = employees[8].age = 30;
    employees[9].age = 50;

    CONTAINER expectedResult =
        { {"manager", 30 },
          {"technician",  18 }
        };

    Reducer<Employee, int> reducer(1000, 
                                   [](int minAge, Employee e) { return minAge > e.age ? e.age : minAge; },
                                   [](int min, int workerResult) { return min > workerResult ? workerResult : min; });

    CONTAINER result = 
        Pipe()
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, std::string, int>(reducer, [](Employee *e) { return &(e->job_title); });

    std::map<std::string, int> sortedResult;
    for (auto it = result.begin(); it != result.end(); it++) {
    	sortedResult[it->first] = it->second;
    }

    REQUIRE(result.size() == expectedResult.size());
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = sortedResult[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}
