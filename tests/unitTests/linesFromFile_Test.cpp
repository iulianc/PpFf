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

const int BLOCK_SIZE = 3;

TEST_CASE( "GetDataFromFileIntoCollectionUsingLinesStaticVersion_", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult=
        { "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
        };
    
    std::string test_file = TEST_FILE;

    std::vector<std::string> currentResult = 
        Flow::source(test_file, BLOCK_SIZE)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "GetDataFromFileIntoCollectionUsingLinesStaticVersion_2", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult =
        {
         "Lorem",
         "ipsum dolor sit",
         "amet, consectetur",
         "adipiscing elit.",
         "Lorem ipsum dolor sit amet, consectetur",
         "adipiscing elit.",
         "Lorem ipsum dolor sit amet,",
         "consectetur",
         "adipiscing elit."
        };

    std::string test_file = TEST_FILE_2;
    std::vector<std::string> currentResult;
    
    for (int bs = 1; bs < 100; bs++ ) {
        currentResult =
            Flow::source(test_file, bs)
            .collect<std::string, std::vector>();
        REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
    }
}

TEST_CASE( "GetDataFromFileIntoCollectionUsingLines_", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult =
        {
         "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
         "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
         "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
        };
    
    std::string test_file = TEST_FILE;
    
    std::vector<std::string> currentResult = 
        Flow
        ::source(test_file, BLOCK_SIZE)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "GetDataFromFileIntoCollectionUsingLinesAvecLignesBlanches_", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult =
        {
         "abc def",
         "",
         "   ghi j",
         "",
         "k",
         "  ",
         "l",
         "",
         "m n o",
         "",
         "p",
         "",
         ""
        };
    
    std::string test_file = "testdata/avec-lignes-blanches.txt";
    
    std::vector<std::string> currentResult;
    
    for( int bs = 1; bs < 29; bs++  ) {
        currentResult = Flow
            ::source(test_file, bs)
            .collect<std::string, std::vector>();

        REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
    }
}
