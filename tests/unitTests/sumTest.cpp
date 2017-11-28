#include "catch.hpp"
#include "../../src/pp/Pipe.hpp"

TEST_CASE("CollectionOfPositiveIntegers", "SumOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    int expectedResult = n * (n - 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    pp::Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .sum<int>();

    REQUIRE(expectedResult == currentResult);
}

TEST_CASE("CollectionOfNegativeIntegers", "SumOperator") {
    int n = 1000;
    std::vector<int> elems(n);
    int expectedResult = - n * (n - 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = -i;
    };

    pp::Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .sum<int>();

    REQUIRE(expectedResult == currentResult);
}

TEST_CASE("CollectionOfFloatElements", "SumOperator") {
    int n = 1000;
    std::vector<float> elems(n);
    float expectedResult = 0.1 * n * (n - 1) / 2.0;

    float temp = 0.0;
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = temp;
        temp += 0.1;
    };

    pp::Pipe pipe;
    float currentResult = pipe
        .source<float>(elems.begin(), elems.end())
        .sum<float>();

    //EXPECT_FLOAT_EQ(expectedResult, currentResult);
    REQUIRE(expectedResult == Approx(currentResult));
}

TEST_CASE("CollectionOfDoubleElements", "SumOperator") {
    int n = 1000;
    std::vector<double> elems(n);
    double expectedResult = 0.1 * n * (n - 1) / 2.0;

    double temp = 0.0;
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = temp;
        temp += 0.1;
    };

    pp::Pipe pipe;
    double currentResult = pipe
        .source<double>(elems.begin(), elems.end())
        .sum<double>();

    //ASSERT_DOUBLE_EQ(expectedResult, currentResult);
    REQUIRE(expectedResult == currentResult);
}

TEST_CASE("SumParallel", "SumOperator") {
    int n = 1000;
    std::vector<int> elems(n);
    int expectedResult = n * (n - 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    pp::Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .sum<int>();

    REQUIRE(expectedResult == currentResult);
}

TEST_CASE("SumOnCollectionTypeDeque", "SumOperator") {
    int n = 10000;
    std::deque<int> elems(n);
    int expectedResult = n * (n + 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i + 1;
    };

    pp::Pipe pipe;
    int currentResult = pipe
        .source<int>(elems.begin(), elems.end())
        .parallel()
        .sum<int>();

    REQUIRE(expectedResult == currentResult);
}
