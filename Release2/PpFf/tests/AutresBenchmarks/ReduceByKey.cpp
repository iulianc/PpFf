#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <functional>
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include<fstream>
#include <locale>
#include <Pipe.hpp>
#include <unordered_map>
#include <ctype.h>

using namespace PpFf;

static int plus1(int count, int _) {
    return count + 1;
};

int main(int argc, char* argv[]) {
    int n = 10;
    int nbThreads = 2;

    if (argc >= 2) {
        n = atoi(argv[1]);
    }
    if (argc >= 3) {
        nbThreads = atoi(argv[2]);
    }
    
    std::vector<int> elems(n * (n+1) / 2);
    std::unordered_map<int,int> expectedResult;
    for (int i = 0; i < n; i++) {
        for (int k = 1; k <= i; k++) {
            elems.push_back(i);
        }
        expectedResult[i] = i;
    }

    Reducer<int, int> reducer(0, plus1, std::plus<int>());


    // EXECUTION PARALLELE.
    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<int,int> result = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(nbThreads)
        .reduceByKey<int, int, int>(reducer);

    auto end = std::chrono::high_resolution_clock::now();

    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        if (expectedResult[it->first] != result[it->first]) {
            printf( "Pas ok pour %d: result = %d vs. expectedResult = %d\n", 
                    it->first, result[it->first], expectedResult[it->first] );
        }
        break;
    }
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "Temps parallele (nbThreads = %d) = %ld\n", nbThreads, duration_ms );

    // EXECUTION SEQUENTIELLE.
    std::unordered_map<int,int> resultSeq;
    begin = std::chrono::high_resolution_clock::now();
    for (auto it = elems.begin(); it != elems.end(); it++) {
        std::unordered_map<int,int>::const_iterator got = resultSeq.find(*it);
        if (got == resultSeq.end()) {
            resultSeq[*it] = 0;
        } 
        resultSeq[*it] += 1;
    }
    end = std::chrono::high_resolution_clock::now();

    for (auto it = expectedResult.begin(); it != expectedResult.end(); it++) {
        if (expectedResult[it->first] != resultSeq[it->first]) {
            printf( "Pas ok pour %d: result = %d vs. expectedResult = %d\n", 
                    it->first, resultSeq[it->first], expectedResult[it->first] );
        }
        break;
    }

    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "Temps sequentiel = %ld\n", duration_ms );

    return 0;
}
