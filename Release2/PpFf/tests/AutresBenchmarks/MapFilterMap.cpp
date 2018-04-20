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

int main(int argc, char* argv[]) {
    int n = 10;
    int nbThreads = 2;

    if (argc >= 2) {
        n = atoi(argv[1]);
    }
    if (argc >= 3) {
        nbThreads = atoi(argv[2]);
    }
    
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

    // EXECUTION PARALLELE.
    auto begin = std::chrono::high_resolution_clock::now();

    std::vector<int> currentResult = 
        Pipe()
        .source<int>(elems.begin(), elems.end())
        .parallel(nbThreads)
        .map<int, int>(fois10)
        .find<int>(divise20)
        .map<int, int>(sommeJusqua)
        .collect<int, std::vector>();

    auto end = std::chrono::high_resolution_clock::now();

    std::sort(currentResult.begin(), currentResult.end());
    for (int i = 0; i < nbResults; i++) {
        if (currentResult[i] != expectedResult[i]) {
            printf( "Pas ok pour %d: currentResult = %d vs. expectedResult = %d\n", i, currentResult[i], expectedResult[i] );
            break;
        }
    }
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "Temps parallele (nbThreads = %d) = %ld\n", nbThreads, duration_ms );



    // EXECUTION SEQUENTIELLE.
    begin = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        elems[i] = i; 
    }
    for (int i = 0; i < n; i++) {
        elems[i] = *fois10(&elems[i]);
    }
    nbResults = 0;
    for (int i = 0; i < n; i++) {
        if (divise20(&elems[i])) {
            elems[nbResults] = elems[i];
            nbResults += 1;
        }
    }
    for (int i = 0; i < nbResults; i++) {
        elems[i] = *sommeJusqua(&elems[i]);
    }
    end = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < nbResults; i++) {
        if (elems[i] != expectedResult[i]) {
            printf( "Pas ok pour %d: elems = %d vs. expectedResult = %d\n", i, elems[i], expectedResult[i] );
            break;
        }
    }
    duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "Temps sequentiel = %ld\n", duration_ms );

    return 0;
}
