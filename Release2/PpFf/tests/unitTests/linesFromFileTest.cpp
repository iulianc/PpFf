#include "catch.hpp"
#include "../../src/Flow.hpp"
#include <string>
#include <vector>
#include <list>
#include "utility.hpp"
#include <algorithm>
#include <map>
#include <iostream>

using namespace PpFf;

#ifndef TEST_FILE
#define TEST_FILE "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt"
#endif

TEST_CASE( "GetDataFromFileIntoCollectionUsingLinesStaticVersion", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    };

    std::string test_file = TEST_FILE;

    std::vector<std::string> currentResult = 
        Flow::source(test_file)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "GetDataFromFileIntoCollectionUsingLines", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    };

    std::string test_file = TEST_FILE;

    std::vector<std::string> currentResult = 
        Flow
        ::source(test_file)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "GetDataFromFileIntoCollectionUsingLinesAvecLignesBlanches", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult = {
        "abc def", "",
        "   ghi j", "",
        "k", "  ",
        "l", "",
        "m n o", "",
        "p", "", ""
    };

    std::string test_file = "testdata/avec-lignes-blanches.txt";
    //std::string test_file = "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/avec-lignes-blanches.txt";

    std::vector<std::string> currentResult = 
        Flow
        ::source(test_file)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}
