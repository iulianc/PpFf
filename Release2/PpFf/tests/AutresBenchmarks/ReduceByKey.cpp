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
    int n = 100;
    int nbThreads = 2;

    if (argc >= 2) {
        n = atoi(argv[1]);
    }
    if (argc >= 3) {
        nbThreads = atoi(argv[2]);
    }

    // On construit les donnees a traiter et le resultat attendu.
    std::vector<int> elems;
    std::unordered_map<int,int> expectedResult;
    for (int i = 1; i <= n; i++) {
        for (int k = 1; k <= i; k++) {
            elems.push_back(i);
        }
        expectedResult[i] = i;
    }

    // EXECUTION SEQUENTIELLE.
    std::unordered_map<int,int> resultSeq;
    auto begin = std::chrono::high_resolution_clock::now();
    for (auto it: elems) {
        if (resultSeq.find(it) == resultSeq.end()) {
            resultSeq[it] = 0;
        } 
        resultSeq[it] += 1;
    }
    auto end = std::chrono::high_resolution_clock::now();

    for (auto it: expectedResult) {
        if (expectedResult[it.first] != resultSeq[it.first]) {
            printf( "Pas ok pour %d: result = %d vs. expectedResult = %d\n", 
                    it.first, resultSeq[it.first], expectedResult[it.first] );
            break;
        }
    }

    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "Temps sequentiel = %ld\n", duration_ms );


    // EXECUTION PARALLELE.
    Reducer<int, int> reducer(0, plus1, std::plus<int>());
    begin = std::chrono::high_resolution_clock::now();
    
    std::unordered_map<int,int> resultPar = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(nbThreads)
        .reduceByKey<int, int, int>(reducer);

    end = std::chrono::high_resolution_clock::now();

    for (auto& it: expectedResult) {
        if (expectedResult[it.first] != resultPar[it.first]) {
            printf( "Pas ok pour %d: result = %d vs. expectedResult = %d\n", 
                    it.first, resultPar[it.first], expectedResult[it.first] );
            break;
        }
    }
    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "Temps parallele (nbThreads = %d) = %ld\n", nbThreads, duration_ms );
    printf( "\n" );

    return 0;
}
