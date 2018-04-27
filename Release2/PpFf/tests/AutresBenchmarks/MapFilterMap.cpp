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
    // On definit les parametres d'execution.
    unsigned int n = 10;
    unsigned int nbThreads = 0; // Execution sequentielle par defaut.

    if (argc >= 2) {
        n = atoi(argv[1]);
    }
    if (argc >= 3) {
        nbThreads = atoi(argv[2]);
    }

    // On definit les elements a traiter et le resultat attendu.
    std::vector<int> elems(n);
    std::vector<int> attendus;
    for (unsigned int i = 0; i < n; i++) {
        elems[i] = i;
        if ((10 * i) % 20 == 0) {
            int i10 = 10 * i;
            attendus.push_back(*sommeJusqua(&i10));
        }
    }
    
    std::vector<int> obtenus;

    auto begin = std::chrono::high_resolution_clock::now();
    if (nbThreads == 0) {
        // EXECUTION SEQUENTIELLE.
        for (unsigned int i = 0; i < n; i++) {
            elems[i] = i; 
        }
        std::vector<int> elems1;
        for (unsigned int i = 0; i < n; i++) {
            elems1.push_back(*fois10(&elems[i]));
        }
        std::vector<int> elems2;
        for (unsigned int i = 0; i < elems1.size(); i++) {
            if (divise20(&elems1[i])) {
                elems2.push_back(elems1[i]);
            }
        }
        for (unsigned int i = 0; i < elems2.size(); i++) {
            obtenus.push_back(*sommeJusqua(&elems2[i]));
        }
    } else {
        // EXECUTION PARALLELE.
        obtenus = 
            Pipe()
            .source<int>(elems.begin(), elems.end())
            .parallel(nbThreads)
            .map<int, int>(fois10)
            .find<int>(divise20)
            .map<int, int>(sommeJusqua)
            .collect<int, std::vector>();
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    std::sort(obtenus.begin(), obtenus.end());
    
    if (obtenus.size() != attendus.size()) {
        printf( "Tailles pas ok: obtenus = %lu vs. attendus = %lu\n", 
                obtenus.size(), attendus.size() );
    }

    for (unsigned int i = 0; i < attendus.size(); i++) {
        if (obtenus[i] != attendus[i]) {
            printf( "Pas ok pour %d: obtenus = %d vs. attendus = %d\n", i, obtenus[i], attendus[i] );
            break;
        }
    }
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "%ld\n", duration_ms );

    return 0;
}
