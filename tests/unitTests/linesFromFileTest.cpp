#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include <list>
#include <vector>
#include <map>
#include "utility.hpp"
#include <algorithm>

#ifndef TEST_FILE
#define TEST_FILE "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt"
#endif


TEST_CASE( "GetDataFromFileIntoCollectionUsingLines", "LinesFromFileOperator" ) {
    std::vector<std::string> expectedResult = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
    };
    
    std::string test_file = TEST_FILE;
    typedef std::vector<std::string> vec_type;

    pp::Pipe pipe;
    std::vector<std::string> currentResult = pipe
        .linesFromFile(test_file)
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}
