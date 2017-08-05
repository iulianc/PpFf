#include "gtest/gtest.h"
#include "../../src/pp/Pipe.hpp"

TEST(SumOperator, CollectionOfPositiveIntegers) {
	std::vector< int > elems(10);
	int expectedResult = 45;
	int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.sum< int >();

    EXPECT_EQ(expectedResult, currentResult);
}

TEST(SumOperator, CollectionOfNegativeIntegers) {
	std::vector< int > elems(10);
	int expectedResult = -45;
	int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = -i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.sum< int >();

    EXPECT_EQ(expectedResult, currentResult);
}

TEST(SumOperator, CollectionOfFloatElements) {
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

	EXPECT_FLOAT_EQ(expectedResult, currentResult);
}

TEST(SumOperator, CollectionOfDoubleElements) {
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

	ASSERT_DOUBLE_EQ(expectedResult, currentResult);
}

TEST(SumOperator, Parallel) {
	std::vector< int > elems(10);
	int expectedResult = 45;
	int currentResult = 0;

	for(unsigned int i = 0; i < elems.size(); i++){
		elems[i] = i;
	};

	pp::Pipe pipe;
	currentResult = pipe.source< int >(elems.begin(), elems.end())
					.parallel()
					.sum< int >();

    EXPECT_EQ(expectedResult, currentResult);
}

TEST(SumOperator, CollectionTypeDeque) {
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

    EXPECT_EQ(expectedResult, currentResult);
}
