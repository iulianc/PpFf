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
#include <Flow.hpp>
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

    // On execute la version appropriee.
    std::unordered_map<int,int> obtenu;

    auto begin = std::chrono::high_resolution_clock::now();
    if (nbThreads == 0) {
        // EXECUTION SEQUENTIELLE.
        for (auto it: elems) {
            if (obtenu.find(it) == obtenu.end()) {
                obtenu[it] = 0;
            } 
            obtenu[it] += 1;
        }
    } else {
        // EXECUTION PARALLELE.
        Reducer<int, int> reducer(0, plus1, std::plus<int>());
        obtenu = 
            Flow
            ::source<int>(elems.begin(), elems.end())
            .parallel(nbThreads)
            .reduceByKey<int, int, int>(reducer);
    }
    auto end = std::chrono::high_resolution_clock::now();

    for (auto it: expectedResult) {
        if (expectedResult[it.first] != obtenu[it.first]) {
            fprintf( stderr, 
                     "Pas ok pour %d: result = %d vs. expectedResult = %d\n", 
                     it.first, obtenu[it.first], expectedResult[it.first] );
            break;
        }
    }

    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "%ld\n", duration_ms );
    return 0;
}
