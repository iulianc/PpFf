#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include <list>
#include <vector>
#include <map>
#include "utility.hpp"
#include <algorithm>


TEST_CASE( "GetDataFromFileIntoCollection", "SourceFromFileOperator" ) {
    std::vector<std::string> expectedResult = {"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit."};
    //std::string path = "testdata/lorem.txt"; // Chemin relatif, pour que cela fonctionne partout!
    std::string path = "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt";

    typedef std::vector<std::string> vec_type;

    pp::Pipe pipe;
    std::vector<std::string> currentResult = pipe
        .sourceFromFile<std::vector>(path, " ")
        .flatMap<vec_type, std::string>()
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "GetDataFromFileByComma", "SourceFromFileOperator" ) {
    std::vector<std::string> expectedResult = {"Lorem ipsum dolor sit amet", "consectetur adipiscing elit.", "Lorem ipsum dolor sit amet", "consectetur adipiscing elit.", "Lorem ipsum dolor sit amet", "consectetur adipiscing elit."};
    //std::string path = "testdata/lorem.txt"; // Chemin relatif, pour que cela fonctionne partout!
    std::string path = "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt";

    typedef std::vector<std::string> vec_type;

    pp::Pipe pipe;
    std::vector<std::string> currentResult = pipe
        .sourceFromFile<std::vector>(path, ", ")
        .flatMap<vec_type, std::string>()
        .collect<std::string, std::vector>();

    REQUIRE_THAT( currentResult, Catch::Equals(expectedResult) );
}

TEST_CASE( "CountWords", "SourceFromFileOperator" ) {
    std::map<std::string, int> expectedResult = {
        {"adipiscing", 3},
        {"amet,", 3},
        {"consectetur", 3},
        {"dolor", 3},
        {"elit.", 3},
        {"ipsum", 3},
        {"lorem", 3},
        {"sit", 3}
    };

    //std::string path = "testdata/lorem.txt";
    std::string path = "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt";

    typedef std::vector<std::string> vec_type;

    pp::Pipe pipe;
    std::map<std::string, int> currentResult = pipe
        .sourceFromFile<std::vector>(path, " ")
        .flatMap<vec_type, std::string>()
        .map<std::string, std::string>( [](std::string *data) ->std::string*
            {
                transform(data->begin(),
                          data->end(),
                          data->begin(),
                          [](unsigned char c) { return std::tolower(c); } );
                return data;
            } )
        .groupByKey<std::string, std::string, int>([](int &count, std::string *data) ->void { count = count+1; } );

    REQUIRE(currentResult.size() == expectedResult.size());

    for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
        REQUIRE(expectedResult[it->first] == it->second);
    }
}


