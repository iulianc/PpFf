#include "catch.hpp"
#include "../../src/Flow.hpp"

using namespace PpFf;

TEST_CASE("CollectionOfPositiveIntegers", "SumOperator") {
    int n = 10000;
    std::vector<int> elems(n);
    int expectedResult = n * (n - 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .sum<int>();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("CollectionOfNegativeIntegers", "SumOperator") {
    int n = 1000;
    std::vector<int> elems(n);
    int expectedResult = - n * (n - 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = -i;
    };

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .sum<int>();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("CollectionOfFloatElements", "SumOperator") {
    int n = 2000;
    std::vector<float> elems(n);
    float expectedResult = 0.1 * n * (n - 1) / 2.0;

    float temp = 0.0;
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = temp;
        temp += 0.1;
    };

    float currentResult = 
        Flow
        ::source<float>(elems.begin(), elems.end())
        .sum<float>();

    REQUIRE(currentResult == Approx(expectedResult));
}

TEST_CASE("CollectionOfDoubleElements", "SumOperator") {
    int n = 10000;
    std::vector<double> elems(n);
    double expectedResult = 0.1 * n * (n - 1) / 2.0;

    double temp = 0.0;
    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = temp;
        temp += 0.1;
    };

    double currentResult = 
        Flow
        ::source<double>(elems.begin(), elems.end())
        .sum<double>();

    REQUIRE(currentResult == Approx(expectedResult));
}

TEST_CASE("SumParallel", "SumOperator") {
    int n = 1000;
    std::vector<int> elems(n);
    int expectedResult = n * (n - 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i;
    };

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel(4)
        .sum<int>();

    REQUIRE(currentResult == expectedResult);
}

TEST_CASE("SumOnCollectionTypeDeque", "SumOperator") {
    int n = 10000;
    std::deque<int> elems(n);
    int expectedResult = n * (n + 1) / 2;

    for (unsigned int i = 0; i < elems.size(); i++) {
        elems[i] = i + 1;
    };

    int currentResult = 
        Flow
        ::source<int>(elems.begin(), elems.end())
        .parallel()
        .sum<int>();

    REQUIRE(currentResult == expectedResult);
}
