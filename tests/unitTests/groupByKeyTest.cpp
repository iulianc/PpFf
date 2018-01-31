#include "catch.hpp"
#include <string>
#include <vector>
#include "../../src/pp/Pipe.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
// Fonctions auxiliaires pour factoriser le code des tests ci-bas.
//////////////////////////////////////////////////////////////////////////

static int ageForID( int id ) {
    switch( id % 10 ) {
    case 0: case 1: case 2:
        return 22;
    case 3: case 4:
        return 18;
    case 5: case 6: case 7:
        return 55;
    case 8:
        return 33;
    case 9:
        return 44;
    }
    return 0;
}

template<typename K>
void assertContainerEquals( MapType<K,int> result, MapType<K,int> expectedResult ) {
    REQUIRE(result.size() == expectedResult.size());

    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        int resultValue = result[it->first];
        int expectedResultValue = it->second;

        REQUIRE(resultValue == expectedResultValue);
    }
}

template<typename K, typename V>
void assertContainerEquals( MapType<K,V> result, MapType<K,V> expectedResult ) {
    REQUIRE(result.size() == expectedResult.size());

    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        V resultValue = result[it->first];
        V expectedResultValue = it->second;

        REQUIRE_THAT(resultValue, Catch::Equals(expectedResultValue));
    }
}

template<typename K, typename V, typename Func>
void assertVectorContainerEquals( MapType<K,std::vector<V>> result, 
                                  MapType<K,std::vector<V>> expectedResult,
                                  Func f ) {
    REQUIRE(result.size() == expectedResult.size());
    
    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        std::vector<V> resultValue = result[it->first];
        std::vector<V> expectedResultValue = it->second;
        
        REQUIRE(resultValue.size() == expectedResultValue.size());
        std::sort(resultValue.begin(), resultValue.end(), 
                  ([f](V v1, V v2) {return f(v1) < f(v2);}));

        for (unsigned int i = 0; i < expectedResultValue.size(); i++) {
            REQUIRE(f(resultValue[i]) == f(expectedResultValue[i]));
        }
    }
}
//////////////////////////////////////////////////////////////////////////

TEST_CASE( "GroupByKeyACollectionTypeVector", "GroupByKeyOperator" ) {
    typedef std::vector<std::string> VALUE;
    typedef MapType<std::string, VALUE> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", 
                                         "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult;
    expectedResult["Employee3"] = {"Employee3", "Employee3", "Employee3", "Employee3"};
    expectedResult["Employee6"] = {"Employee6", "Employee6"};
    expectedResult["Employee9"] = {"Employee9", "Employee9"};

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(strElems.begin(), strElems.end())
        .groupByKey<std::string>();

    assertContainerEquals(result, expectedResult);
}

TEST_CASE( "GroupByKeyACollectionTypeVectorBigNombreElements", "GroupByKeyOperator" ) {
    typedef std::vector<std::string> VALUE;
    typedef MapType<std::string, VALUE> CONTAINER;

    int n = 200;
    CONTAINER expectedResult;
    std::vector<std::string> elems;
    for (int i = 0; i < n; i++) {
        std::string en = "Employee" + ConvertNumberToString(i) ;
        std::vector<std::string> lesElems;
        for (int j = 0; j <= i; j++ ) {
            elems.push_back(en);
            lesElems.push_back(en);
        }
        expectedResult[en] = lesElems;
    }

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(elems.begin(), elems.end())
        .groupByKey<std::string>();

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByAgeACollectionEmployees", "GroupByKeyOperator" ) {
    typedef std::vector<Employee> VALUE;
    typedef MapType<int, VALUE> CONTAINER;

    unsigned int noEmployees = 10;
    VALUE employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(ageForID(i),
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult =
        { {22, {employees[0], employees[1], employees[2]}},
          {18, {employees[3], employees[4]}},
          {55, {employees[5], employees[6], employees[7]}},
          {33, {employees[8]}},
          {44, {employees[9]}}
        };

    pp::Pipe pipe;
    CONTAINER result;

    SECTION( "Appel avec deux arguments" ) {
        result = pipe
            .source<Employee>(employees.begin(), employees.end())
            .groupByKey<Employee, int, Employee>( [](Employee *e) { return &e->age; },
                                                  [](Employee *e) { return e; });
        
        assertVectorContainerEquals(result, 
                                    expectedResult, 
                                    [](Employee e){ return e.name; });
    }
    SECTION( "Appel semblable mais avec un seul argument, produisant le meme resultat" ) {
        result = pipe
            .source<Employee>(employees.begin(), employees.end())
            .groupByKey<Employee, int, Employee>( [](Employee *e) { return &e->age; } );

        assertVectorContainerEquals(result, 
                                    expectedResult, 
                                    [](Employee e){ return e.name; });
    }
}


TEST_CASE( "GroupByXGivingCollectionOfTypes", "GroupByKeyOperator" ) {
    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(ageForID(i), "Employee" + ConvertNumberToString(i), ageForID(i) * 10);
        employees.push_back(employee);
    };

    MapType<int, std::vector<int>> expectedResult;
    MapType<int, std::vector<int>> result;
    pp::Pipe pipe;

    SECTION( "K = age; V = age" ) {
        expectedResult = { {22, {22, 22, 22}}, {18, {18, 18}}, {55, {55, 55, 55}}, {33, {33}}, {44, {44}} };
        result = pipe
            .source<Employee>(employees.begin(), employees.end())
            .groupByKey<Employee, int, int>( [](Employee *e) { return &e->age; },
                                             [](Employee *e) { return &e->age; });
    }

    SECTION( "K = age; V = salaire" ) {
        expectedResult = { {22, {220, 220, 220}}, {18, {180, 180}}, {55, {550, 550, 550}}, {33, {330}}, {44, {440}} };
        result = pipe
            .source<Employee>(employees.begin(), employees.end())
            .groupByKey<Employee, int, int>( [](Employee *e) { return &e->age; },
                                             [](Employee *e) { return &e->salary; });
    }

    SECTION( "K = salaire; V = age" ) {
        expectedResult = { {220, {22, 22, 22}}, {180, {18, 18}}, {550, {55, 55, 55}}, {330, {33}}, {440, {44}} };
        result = pipe
            .source<Employee>(employees.begin(), employees.end())
            .groupByKey<Employee, int, int>( [](Employee *e) { return &e->salary; },
                                             [](Employee *e) { return &e->age; });
    }

    assertVectorContainerEquals(result, expectedResult, []( int x ) { return x; } );
}

TEST_CASE( "GroupByAgeAndCountEmployees", "GroupByKeyOperator" ) {
    typedef MapType<int, int> CONTAINER;

    std::vector<Employee> employees;
    unsigned int NB = 1000;
    unsigned int noEmployees = 10 * NB;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(ageForID(i),
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult = {{22, 3*NB}, {18, 2*NB}, {55, 3*NB}, {33, 1*NB}, {44, 1*NB}};
    

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, int, Aggregates::OperatorCount>( [](Employee *e) { return &e->age; } );
    
    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByKeyACollectionTypeVectorParallel", "GroupByKeyOperator" ) {
    typedef std::vector<std::string> VALUE;
    typedef MapType<std::string, VALUE> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", 
                                         "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult;
    expectedResult["Employee3"] = {"Employee3", "Employee3", "Employee3", "Employee3"};
    expectedResult["Employee6"] = {"Employee6", "Employee6"};
    expectedResult["Employee9"] = {"Employee9", "Employee9"};

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(strElems.begin(), strElems.end())
        .parallel(4)
        .groupByKey<std::string>();

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByKeyACollectionTypeVectorBigNombreElementsParallel", "GroupByKeyOperator" ) {
    typedef std::vector<std::string> VALUE;
    typedef MapType<std::string, VALUE> CONTAINER;

    int n = 200;
    CONTAINER expectedResult;
    std::vector<std::string> elems;
    for (int i = 0; i < n; i++) {
        std::string en = "Employee" + ConvertNumberToString(i) ;
        std::vector<std::string> lesElems;
        for( int j = 0; j <= i; j++ ) {
            elems.push_back(en);
            lesElems.push_back(en);
        }
        expectedResult[en] = lesElems;
    }

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(elems.begin(), elems.end())
        .parallel(4)
        .groupByKey<std::string>();

    assertContainerEquals(result, expectedResult);
}

TEST_CASE( "GroupByAgeACollectionEmployeesParallel", "GroupByKeyOperator" ) {
    typedef std::vector<Employee> VALUE;
    typedef MapType<int, VALUE> CONTAINER;

    unsigned int noEmployees = 10;
    VALUE employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(ageForID(i),
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult =
        { {22, {employees[0], employees[1], employees[2]}},
          {18, {employees[3], employees[4]}},
          {55, {employees[5], employees[6], employees[7]}},
          {33, {employees[8]}},
          {44, {employees[9]}}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, int, Employee>( [](Employee *e) { return new int(e->age); } );


    assertVectorContainerEquals(result, expectedResult, [](Employee e){ return e.name; });
}


TEST_CASE( "GroupByAgeAndCountEmployeesParallel", "GroupByKeyOperator" ) {
    typedef MapType<int, int> CONTAINER;

    std::vector<Employee> employees;
    unsigned int NB = 1000;
    unsigned int noEmployees = 10 * NB;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(ageForID(i),
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult = {{22, 3*NB}, {18, 2*NB}, {55, 3*NB}, {33, 1*NB}, {44, 1*NB}};


    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, int, int, Aggregates::OperatorCount>( [](Employee *e) { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}

//****************Testing functions Aggregates**************************************
TEST_CASE( "GroupByKeyCountElementsContainer", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", 
                                         "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult = {{"Employee9", 2}, {"Employee6", 3}, {"Employee3", 4}};


    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(strElems.begin(), strElems.end())
        .groupByKey<std::string, std::string, int, Aggregates::OperatorCount>();
    
    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByAgeCountEmployees", "GroupByKeyOperator" ) {
    typedef MapType<int, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(ageForID(i),
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult =
        { {18, 2},
          {22, 3},
          {33, 1},
          {44, 1},
          {55, 3}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, int, int, Aggregates::OperatorCount>( [](Employee *e) { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesSumSalary", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, std::string, int, Aggregates::OperatorSum>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->salary; });


    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesAverageAge", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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
        { {"manager", 40},
          {"technician",  23}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, std::string, int, Aggregates::OperatorAvg>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->age; } );


    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesMaxAge", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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
        { {"manager", 50},
          {"technician",  30}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, std::string, int, Aggregates::OperatorMax>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesMinAge", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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
        { {"manager", 30},
          {"technician",  18}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .groupByKey<Employee, std::string, int, Aggregates::OperatorMin>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}

//Testing function Aggregate in Parallel
TEST_CASE( "GroupByKeyCountElementsContainerParallel", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

    std::vector<std::string> strElems = {"Employee3","Employee6", "Employee9", "Employee3", "Employee6", 
                                         "Employee6", "Employee9", "Employee3", "Employee3"};

    CONTAINER expectedResult = {{"Employee9", 2}, {"Employee6", 3}, {"Employee3", 4}};


    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<std::string>(strElems.begin(), strElems.end())
        .parallel(4)
        .groupByKey<std::string, std::string, int, Aggregates::OperatorCount>();

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByAgeCountEmployeesParallel", "GroupByKeyOperator" ) {
    typedef MapType<int, int> CONTAINER;

    unsigned int noEmployees = 10;
    std::vector<Employee> employees;
    for (unsigned int i = 0; i < noEmployees; i++) {
        Employee employee(ageForID(i),
                          "Employee" + ConvertNumberToString(i),
                          i % 3 == 0 ? i * 100 : i * 10);
        employees.push_back(employee);
    };

    CONTAINER expectedResult =
        { {18, 2},
          {22, 3},
          {33, 1},
          {44, 1},
          {55, 3}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, int, int, Aggregates::OperatorCount>( [](Employee *e) ->int* { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesSumSalaryParallel", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, std::string, int, Aggregates::OperatorSum>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->salary; });

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesAverageAgeParallel", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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
        { {"manager", 40},
          {"technician",  23}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, std::string, int, Aggregates::OperatorAvg>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesMaxAgeParallel", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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
        { {"manager", 50},
          {"technician",  30}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, std::string, int, Aggregates::OperatorMax>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}


TEST_CASE( "GroupByJobTitleEmployeesMinAgeParallel", "GroupByKeyOperator" ) {
    typedef MapType<std::string, int> CONTAINER;

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
        { {"manager", 30},
          {"technician",  18}
        };

    pp::Pipe pipe;
    CONTAINER result = pipe
        .source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .groupByKey<Employee, std::string, int, Aggregates::OperatorMin>( [](Employee *e) { return &e->job_title; },
                                                                          [](Employee *e) { return &e->age; } );

    assertContainerEquals(result, expectedResult);
}
