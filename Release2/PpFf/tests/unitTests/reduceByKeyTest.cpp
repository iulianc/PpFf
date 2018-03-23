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

void FuncReduce(int *count, std::string *in) {
    *count = *count + 1;
};

TEST_CASE( "ReduceByKeyCountElementsContainer", "ReduceByKeyOperator" ) {
    typedef std::unordered_map<std::string, int> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult = {{"Employee9", 2}, {"Employee6", 3}, {"Employee3", 4}};

    Reducer< std::string, int > reducer(FuncReduce);

    Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(strElems.begin(), strElems.end())
        .reduceByKey<std::string, std::string, int>(reducer);

    //        for (auto it = result.begin(); it != result.end(); it++) {
    //
    //            //std::sort(expectedResultValue.begin(), expectedResultValue.end(), [](Employee e1, Employee e2)->bool {return e1.name < e2.name; } );
    //
    //            std::cout << it->first << ": " << it->second << std::endl;
    //        }

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

    Reducer< std::string, int > reducer(FuncReduce);

    Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(strElems.begin(), strElems.end())
        .reduceByKey<std::string, std::string, int, std::map<std::string, int> >(reducer);


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

    Reducer<Employee, int> reducer( 0, [](int *count, Employee *e) ->void { *count = *count + 1; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, int, int>( [](Employee *e) ->int* { return &(e->age); }, reducer );

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

    Reducer<Employee, int> reducer( 0, [](int *totalSalary, Employee *e) ->void { *totalSalary += e->salary; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, std::string, int>( [](Employee *e) ->std::string* { return &(e->job_title); }, reducer );


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

    Reducer<Employee, int> reducer( 0, [](int *maxAge, Employee *e) ->void { if(*maxAge < e->age) *maxAge = e->age; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, std::string, int>( [](Employee *e) ->std::string* { return &(e->job_title); }, reducer );


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

    Reducer<Employee, int> reducer( 1000, [](int *minAge, Employee *e) ->void { if(*minAge > e->age) *minAge = e->age; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .reduceByKey<Employee, std::string, int>( [](Employee *e) ->std::string* { return &(e->job_title); }, reducer );


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

    Reducer< std::string, int > reducer(FuncReduce, [](int *total, int *workerResult) ->void { *total += *workerResult; } );

    Pipe pipe;
    CONTAINER result = pipe
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

    Reducer< Employee, int > reducer([](int *count, Employee *e) ->void { *count += 1; }, [](int *total, int *workerResult) ->void { *total += *workerResult; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, int, int>( [](Employee *e) ->int* { return &(e->age); }, reducer );

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

    Reducer< Employee, int > reducer(0, [](int *salary, Employee *e) ->void { *salary += e->salary; },
                                     [](int *total, int *workerResult) ->void { *total += *workerResult; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, std::string, int>( [](Employee *e) ->std::string* { return &(e->job_title); },
                                                  reducer );

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

    Reducer< Employee, int > reducer(0, [](int *maxAge, Employee *e) ->void { if(*maxAge < e->age) *maxAge = e->age; },
                                     [](int *max, int *workerResult) ->void { if(*max < *workerResult) *max = *workerResult; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, std::string, int>( [](Employee *e) ->std::string* { return &(e->job_title); },
                                                  reducer );

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

    Reducer< Employee, int > reducer(1000, [](int *minAge, Employee *e) ->void { if(*minAge > e->age) *minAge = e->age; },
                                     [](int *min, int *workerResult) ->void { if(*min > *workerResult) *min = *workerResult; } );

    Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .reduceByKey<Employee, std::string, int>( [](Employee *e) ->std::string* { return &(e->job_title); },
                                                  reducer );

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
