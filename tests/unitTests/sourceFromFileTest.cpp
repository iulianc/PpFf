#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include <list>
#include <vector>
#include <map>
#include "utility.hpp"
#include <algorithm>


TEST_CASE( "GetDataFromFileIntoCollection", "SourceFromFileOperator" ) {
	std::vector< std::string > expectedResult;
	expectedResult = {"Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit.", "Lorem", "ipsum", "dolor", "sit", "amet,", "consectetur", "adipiscing", "elit."};
	std::vector< std::string > currentResult;
	std::string path = "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt";

	typedef std::vector< std::string > vec_type;

	pp::Pipe pipe;
	currentResult = pipe.sourceFromFile< std::vector >(path, " ")
							.flatMap< vec_type, std::string >()
							.collect< std::string, std::vector >();

	for(unsigned int i = 0; i < currentResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}

TEST_CASE( "GetDataFromFileByComma", "SourceFromFileOperator" ) {
	std::vector< std::string > expectedResult;
	expectedResult = {"Lorem ipsum dolor sit amet", "consectetur adipiscing elit.", "Lorem ipsum dolor sit amet", "consectetur adipiscing elit.", "Lorem ipsum dolor sit amet", "consectetur adipiscing elit."};
	std::vector< std::string > currentResult;
	std::string path = "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt";

	typedef std::vector< std::string > vec_type;

	pp::Pipe pipe;
	currentResult = pipe.sourceFromFile< std::vector >(path, ", ")
							.flatMap< vec_type, std::string >()
							.collect< std::string, std::vector >();

	for(unsigned int i = 0; i < currentResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}

TEST_CASE( "CountWords", "SourceFromFileOperator" ) {
	std::map< std::string, int > expectedResult = {{"adipiscing", 3}, {"amet,", 3}, {"consectetur", 3}, {"dolor", 3}, {"elit.", 3}, {"ipsum", 3}, {"lorem", 3}, {"sit", 3}};

	std::map< std::string, int > currentResult;
	std::string path = "/home/iuly/WorkplaceEclipse/PpFf/tests/unitTests/testdata/lorem.txt";

	typedef std::vector< std::string > vec_type;
//	typedef std::string (*myFunc)(std::string);
//	typedef int (*biOp)(int, std::string);

	pp::Pipe pipe;
	currentResult = pipe.sourceFromFile< std::vector >(path, " ")
						.flatMap< vec_type, std::string >()
						.map< std::string, std::string >(([](std::string data)->std::string {transform(data.begin(), data.end(), data.begin(), [](unsigned char c){ return std::tolower(c); });
																									return data;}))
						.groupByKey< std::string, std::string, int >(0, ([](int count, std::string data)->int {return count+1;}));

	std::map< std::string, int >::iterator it = currentResult.begin();
	while(it != currentResult.end())
	{
		std::string currentResultKey = it->first;

		int currentResultValue = it->second;
		int expectedResultValue = expectedResult[currentResultKey];

		REQUIRE(expectedResultValue == currentResultValue);

		//std::cout << "<" << currentResultKey << " " << ConvertNumberToString(currentResultValue) << ">" << std::endl;

		it++;
	}
}
