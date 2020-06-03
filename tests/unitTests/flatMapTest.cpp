
#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

typedef std::vector<std::string> Languages;
typedef std::vector<int> Integers;

Languages* GetLanguages(Employee *employee) {
    Languages *languages = new Languages();
    *languages = employee->languages;
    return languages;
}

TEST_CASE( "FlatCollectionTypeVector", "FlatMapOperator" ) {
    unsigned int noElems = 3;
    Integers elems_collection1(noElems),
        elems_collection2(noElems),
        elems_collection3(noElems);
    std::vector<Integers> collection = {elems_collection1, elems_collection2, elems_collection3};
    for (unsigned int i = 0; i < collection.size(); i++) {
        for (unsigned int j = 0; j < collection[i].size(); j++) {
            collection[i][j] = i + j * 3;
        }
    };

    Integers expectedResult = {0, 3, 6, 1, 4, 7, 2, 5, 8};

    Integers currentResult = 
        Flow
        ::source<Integers>(collection.begin(), collection.end())
        .flatten<Integers, int>()
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "FlatCollectionTypeDeque", "FlatMapOperator" ) {
    unsigned int noElems = 3;
    std::deque<int> elems_collection1(noElems),
        elems_collection2(noElems),
        elems_collection3(noElems);
    std::deque<std::deque<int>> collection = {elems_collection1, elems_collection2, elems_collection3};

    for (unsigned int i = 0; i < collection.size(); i++) {
        for (unsigned int j = 0; j < collection[i].size(); j++) {
            collection[i][j] = i + j * 3;
        }
    };

    Integers expectedResult = {0, 3, 6, 1, 4, 7, 2, 5, 8};

    typedef std::deque<int> deque_type;
    Integers currentResult = 
        Flow
        ::source<deque_type>(collection.begin(), collection.end())
        .flatten<deque_type, int>()
        .collect<int, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "FlatCollectionParallel", "FlatMapOperator" ) {
    unsigned int noElems = 3;
    Integers elems_collection1(noElems),
        elems_collection2(noElems),
        elems_collection3(noElems);
    std::vector<Integers> collection = {elems_collection1, elems_collection2, elems_collection3};
    for (unsigned int i = 0; i < collection.size(); i++) {
        for (unsigned int j = 0; j < collection[i].size(); j++) {
            collection[i][j] = i + j * 3;
        }
    };

    Integers expectedResult = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    Integers currentResult =
        Flow
        ::source<Integers>(collection.begin(), collection.end())
        .parallel(4)
        .flatten<Integers, int>()
        .collect<int, std::vector>();
    std::sort(currentResult.begin(), currentResult.end());

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "FlatCollectionApplyingFunction", "FlatMapOperator" ) {
    int noElems = 3;
    std::vector<Employee> employees(noElems);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee("Employee" + ConvertNumberToString(i));
        employees[i] = employee;
    };
    employees[0].languages = {"French", "English"};
    employees[1].languages = {"Chinese", "Arabic", "French"};
    employees[2].languages = {"Spanish", "Portuguese"};

    std::vector<std::string> expectedResult =
        {"French", "English", "Chinese", "Arabic", "French", "Spanish", "Portuguese"};


    std::vector<std::string> currentResult = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .flatMap<Employee, std::string, Languages>(GetLanguages)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE( "FlatCollectionApplyingFunction_", "FlatMapOperator" ) {
    int noElems = 3;
    std::vector<Employee> employees(noElems);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee("Employee" + ConvertNumberToString(i));
        employees[i] = employee;
    };
    employees[0].languages = {"French", "English"};
    employees[1].languages = {"Chinese", "Arabic", "French"};
    employees[2].languages = {"Spanish", "Portuguese"};

    std::vector<std::string> expectedResult =
        {"French", "English", "Chinese", "Arabic", "French", "Spanish", "Portuguese"};


    std::vector<std::string> currentResult = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .flatMap_<Employee, Languages, std::string>(GetLanguages)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE( "FlatCollectionApplyingFunctionInTwoSteps", "FlatMapOperator" ) {
    int noElems = 3;
    std::vector<Employee> employees(noElems);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee("Employee" + ConvertNumberToString(i));
        employees[i] = employee;
    };
    employees[0].languages = {"French", "English"};
    employees[1].languages = {"Chinese", "Arabic", "French"};
    employees[2].languages = {"Spanish", "Portuguese"};

    std::vector<std::string> expectedResult =
        {"French", "English", "Chinese", "Arabic", "French", "Spanish", "Portuguese"};


    std::vector<std::string> currentResult = 
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .map<Employee, Languages>(GetLanguages)
        .flatten<Languages, std::string>()
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE( "FlatCollectionApplyingLambdaFunctionParallel", "FlatMapOperator" ) {
    std::vector<Employee> employees(3);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee("Employee" + ConvertNumberToString(i));
        employees[i] = employee;
    };
    employees[0].languages = {"French", "English"};
    employees[1].languages = {"Chinese", "Arabic", "French"};
    employees[2].languages = {"Spanish", "Portuguese"};

    std::vector<std::string> expectedResult =
        {"Arabic", "Chinese", "English", "French", "French", "Portuguese", "Spanish" };


    std::vector<std::string> currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .flatMap<Employee, std::string, Languages>( [](Employee *empl)
                                                    {
                                                        Languages *languages = new Languages();
                                                        *languages = empl->languages;
                                                        return languages;
                                                    } )
        .collect<std::string, std::vector>();
    std::sort(currentResult.begin(), currentResult.end());

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE( "FlatCollectionApplyingLambdaFunctionParallel_", "FlatMapOperator" ) {
    std::vector<Employee> employees(3);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee("Employee" + ConvertNumberToString(i));
        employees[i] = employee;
    };
    employees[0].languages = {"French", "English"};
    employees[1].languages = {"Chinese", "Arabic", "French"};
    employees[2].languages = {"Spanish", "Portuguese"};

    std::vector<std::string> expectedResult =
        {"Arabic", "Chinese", "English", "French", "French", "Portuguese", "Spanish" };


    std::vector<std::string> currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .flatMap_<Employee, Languages, std::string>( [](Employee *empl)
                                                     {
                                                         Languages *languages = new Languages();
                                                         *languages = empl->languages;
                                                         return languages;
                                                     } )
        .collect<std::string, std::vector>();
    std::sort(currentResult.begin(), currentResult.end());

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE( "FlatCollectionApplyingLambdaFunctionParallelInTwoStages", "FlatMapOperator" ) {
    std::vector<Employee> employees(3);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee("Employee" + ConvertNumberToString(i));
        employees[i] = employee;
    };
    employees[0].languages = {"French", "English"};
    employees[1].languages = {"Chinese", "Arabic", "French"};
    employees[2].languages = {"Spanish", "Portuguese"};

    std::vector<std::string> expectedResult =
        {"Arabic", "Chinese", "English", "French", "French", "Portuguese", "Spanish" };


    std::vector<std::string> currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .map<Employee, Languages>( [](Employee *empl)
                                   {
                                       Languages *languages = new Languages();
                                       *languages = empl->languages;
                                       return languages;
                                   } )
        .flatten<Languages, std::string>()
        .collect<std::string, std::vector>();
    std::sort(currentResult.begin(), currentResult.end());

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}


TEST_CASE( "FlatCollectionGrosNombreElements", "FlatMapOperator" ) {
    int n = 1000;

    std::vector<Employee> employees(n);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee(0, "Employee" + ConvertNumberToString(i), 0, {"French", "English", "Spanish"});
        employees[i] = employee;
    };

    std::vector<std::string> currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .flatMap<Employee, std::string, Languages>( [](Employee *empl)
                                                    {
                                                        Languages *languages = new Languages();
                                                        *languages = empl->languages;
                                                        return languages;
                                                    } )
        .collect<std::string, std::vector>();
    std::sort(currentResult.begin(), currentResult.end());

    for (int i = 0; i < n; i++ ) {
        REQUIRE( currentResult[i] == "English" );
        REQUIRE( currentResult[n + i] == "French" );
        REQUIRE( currentResult[2 * n + i] == "Spanish" );
    }
}
TEST_CASE( "FlatCollectionGrosNombreElements_", "FlatMapOperator" ) {
    int n = 1000;

    std::vector<Employee> employees(n);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee(0, "Employee" + ConvertNumberToString(i), 0, {"French", "English", "Spanish"});
        employees[i] = employee;
    };

    std::vector<std::string> currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .flatMap_<Employee, Languages, std::string>( [](Employee *empl)
                                                     {
                                                         Languages *languages = new Languages();
                                                         *languages = empl->languages;
                                                         return languages;
                                                     } )
        .collect<std::string, std::vector>();
    std::sort(currentResult.begin(), currentResult.end());

    for (int i = 0; i < n; i++ ) {
        REQUIRE( currentResult[i] == "English" );
        REQUIRE( currentResult[n + i] == "French" );
        REQUIRE( currentResult[2 * n + i] == "Spanish" );
    }
}

TEST_CASE( "FlatCollectionGrosNombreElementsInTwoStages", "FlatMapOperator" ) {
    int n = 1000;

    std::vector<Employee> employees(n);
    for (unsigned int i = 0; i < employees.size(); i++) {
        Employee employee(0, "Employee" + ConvertNumberToString(i), 0, {"French", "English", "Spanish"});
        employees[i] = employee;
    };

    std::vector<std::string> currentResult =
        Flow
        ::source<Employee>(employees.begin(), employees.end())
        .parallel(4)
        .map<Employee, Languages>( [](Employee *empl)
                                   {
                                       Languages *languages = new Languages();
                                       *languages = empl->languages;
                                       return languages;
                                   } )
        .flatten<Languages, std::string>()
        .collect<std::string, std::vector>();
    std::sort(currentResult.begin(), currentResult.end());

    for (int i = 0; i < n; i++ ) {
        REQUIRE( currentResult[i] == "English" );
        REQUIRE( currentResult[n + i] == "French" );
        REQUIRE( currentResult[2 * n + i] == "Spanish" );
    }
}
