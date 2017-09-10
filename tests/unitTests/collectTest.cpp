
#include "catch.hpp"
#include <string>
#include "../../src/pp/Pipe.hpp"
#include <list>

TEST_CASE( "ReturnCollectionTypeVector", "CollectOperator" ) {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {0,3,6,9,12,15,18,21,24,27};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef int (*mapF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.map< int, int, mapF >(([](int in)->int {return in * 3;}))
					.collect< int, std::vector >();

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}

TEST_CASE("ReturnCollectionTypeDeque", "CollectOperator") {
	std::vector< int > elems(10);
	std::deque< int > expectedResult(10);
	expectedResult = {0,2,4,6,8,10,12,14,16,18};
	std::deque< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef int (*mapF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.map< int, int, mapF >(([](int in)->int {return in * 2;}))
					.collect< int, std::deque >();

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}


TEST_CASE("ReturnCollectionTypeList", "CollectOperator") {
	std::vector< int > elems(10);
	std::list< int > expectedResult(10);
	expectedResult = {1,2,3,4,5,6,7,8,9,10};
	std::list< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef int (*mapF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.map< int, int, mapF >(([](int in)->int {return in + 1;}))
					.collect< int, std::list >();

	std::list<int>::iterator currentIterator = currentResult.begin();
	for (std::list<int>::iterator expectedIterator = expectedResult.begin(); expectedIterator != expectedResult.end(); expectedIterator++){
		REQUIRE(*expectedIterator == *currentIterator);
		currentIterator++;
	}
}


TEST_CASE("CollectElementsParallel", "CollectOperator") {
	std::vector< int > elems(10);
	std::vector< int > expectedResult(10);
	expectedResult = {0,3,6,9,12,15,18,21,24,27};
	std::vector< int > currentResult;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	typedef int (*mapF)(int);

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.parallel(4)
					.map< int, int, mapF >(([](int in)->int {return in * 3;}))
					.collect< int, std::vector >();

	std::sort(currentResult.begin(), currentResult.end());

	for(unsigned int i = 0; i < expectedResult.size(); i++){
		REQUIRE(expectedResult[i] == currentResult[i]);
	};
}
