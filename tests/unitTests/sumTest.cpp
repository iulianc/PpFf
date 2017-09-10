#include "catch.hpp"
#include "../../src/pp/Pipe.hpp"

TEST_CASE("CollectionOfPositiveIntegers", "SumOperator") {
	std::vector< int > elems(10);
	int expectedResult = 45;
	int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.sum< int >();

	REQUIRE(expectedResult == currentResult);
}

TEST_CASE("CollectionOfNegativeIntegers", "SumOperator") {
	std::vector< int > elems(10);
	int expectedResult = -45;
	int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = -i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.sum< int >();

	REQUIRE(expectedResult == currentResult);
}

TEST_CASE("CollectionOfFloatElements", "SumOperator") {
	std::vector< float > elems(10);
	float expectedResult = 4.5;
	float currentResult = 0.0;

	float temp = 0.0;
	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = temp;
		temp += 0.1;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< float >(elems.begin(), elems.end())
					.sum< float >();

	//EXPECT_FLOAT_EQ(expectedResult, currentResult);
	REQUIRE(expectedResult == Approx(currentResult));
}

TEST_CASE("CollectionOfDoubleElements", "SumOperator") {
	std::vector< double > elems(10);
	double expectedResult = 4.5;
	double currentResult = 0.0;

	double temp = 0.0;
	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = temp;
		temp += 0.1;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< double >(elems.begin(), elems.end())
					.sum< double >();

	//ASSERT_DOUBLE_EQ(expectedResult, currentResult);
	REQUIRE(expectedResult == currentResult);
}

TEST_CASE("SumParallel", "SumOperator") {
	std::vector< int > elems(10);
	int expectedResult = 45;
	int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.parallel(4)
					.sum< int >();

	REQUIRE(expectedResult == currentResult);
}

TEST_CASE("SumOnCollectionTypeDeque", "SumOperator") {
	std::deque< int > elems(10);
	int expectedResult = 55;
	int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i + 1;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.parallel()
					.sum< int >();

	REQUIRE(expectedResult == currentResult);
}
