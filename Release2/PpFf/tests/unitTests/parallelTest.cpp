#include <string>
#include <vector>
#include "../unitTests/catch.hpp"
#include "Employee.hpp"
#include "utility.hpp"
#include "../../src/Pipe.hpp"

using namespace PpFf;

static int* fois10(int *in){
    int* res = (int*) malloc(sizeof(int));
    *res = 10 * *in;

    return res;
};

static bool divise20(int *in){
    return *in % 20 == 0;
};

static int* sommeJusqua(int *in){
    int* res = (int*) malloc(sizeof(int));
    *res = 0;
    for (int i = 1; i <= *in; i++) {
        *res += i / 20 % 2 == 0 ? 1 : 2;
    }

    return res;
};

TEST_CASE( "ParallelWithPipeline", "ParallelTest" ) {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);
    int nbResults = 0;
    for (int i = 0; i < n; i++) {
        elems[i] = i;
        if ((10 * i) % 20 == 0) {
            int i10 = 10 * i;
            expectedResult[nbResults] = *sommeJusqua(&i10);
            nbResults += 1;
        }
    }

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>(fois10)
        .find<int>(divise20)
        .map<int, int>(sommeJusqua)
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    for (int i = 0; i < nbResults; i++) {
        if (currentResult[i] != expectedResult[i]) {
            printf( "Pas ok pour %d: currentResult = %d vs. expectedResult = %d\n", i, currentResult[i], expectedResult[i] );
        }
        break;
    }
}


TEST_CASE( "ChangingThreadNumber", "ParallelTest" ) {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);
    int nbResults = 0;
    for (int i = 0; i < n; i++) {
        elems[i] = i;
        if ((10 * i) % 20 == 0) {
            int i10 = 10 * i;
            expectedResult[nbResults] = *sommeJusqua(&i10);
            nbResults += 1;
        }
    }

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>(fois10)
        .find<int>(divise20)
		.parallel(2)
        .map<int, int>(sommeJusqua)
		.parallel(1)
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    for (int i = 0; i < nbResults; i++) {
        if (currentResult[i] != expectedResult[i]) {
            printf( "Pas ok pour %d: currentResult = %d vs. expectedResult = %d\n", i, currentResult[i], expectedResult[i] );
        }
        break;
    }
}


TEST_CASE( "ParalleWithFarmForEachNode", "ParallelTest" ) {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);
    int nbResults = 0;
    for (int i = 0; i < n; i++) {
        elems[i] = i;
        if ((10 * i) % 20 == 0) {
            int i10 = 10 * i;
            expectedResult[nbResults] = *sommeJusqua(&i10);
            nbResults += 1;
        }
    }

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>(fois10)
		.parallel(2)
        .find<int>(divise20)
		.parallel(4)
        .map<int, int>(sommeJusqua)
		.parallel(2)
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    for (int i = 0; i < nbResults; i++) {
        if (currentResult[i] != expectedResult[i]) {
            printf( "Pas ok pour %d: currentResult = %d vs. expectedResult = %d\n", i, currentResult[i], expectedResult[i] );
        }
        break;
    }
}


TEST_CASE( "ParalleWithPipelineAndFarmForEachNode", "ParallelTest" ) {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);
    int nbResults = 0;
    for (int i = 0; i < n; i++) {
        elems[i] = i;
        if ((10 * i) % 20 == 0) {
            int i10 = 10 * i;
            expectedResult[nbResults] = *sommeJusqua(&i10);
            nbResults += 1;
        }
    }

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>(fois10)
		.parallel(1)
        .find<int>(divise20)
		.parallel(4)
        .map<int, int>(sommeJusqua)
		.parallel(2)
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    for (int i = 0; i < nbResults; i++) {
        if (currentResult[i] != expectedResult[i]) {
            printf( "Pas ok pour %d: currentResult = %d vs. expectedResult = %d\n", i, currentResult[i], expectedResult[i] );
        }
        break;
    }
}


TEST_CASE( "ParalleSerialParallel", "ParallelTest" ) {
    int n = 1000;

    std::vector<int> elems(n);
    std::vector<int> expectedResult(n);
    int nbResults = 0;
    for (int i = 0; i < n; i++) {
        elems[i] = i;
        if ((10 * i) % 20 == 0) {
            int i10 = 10 * i;
            expectedResult[nbResults] = *sommeJusqua(&i10);
            nbResults += 1;
        }
    }

    std::vector<int> currentResult =
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(4)
        .map<int, int>(fois10)
		.parallel(1)
        .find<int>(divise20)
        .map<int, int>(sommeJusqua)
		.parallel(2)
        .collect<int, std::vector>();

    std::sort(currentResult.begin(), currentResult.end());
    for (int i = 0; i < nbResults; i++) {
        if (currentResult[i] != expectedResult[i]) {
            printf( "Pas ok pour %d: currentResult = %d vs. expectedResult = %d\n", i, currentResult[i], expectedResult[i] );
        }
        break;
    }
}

