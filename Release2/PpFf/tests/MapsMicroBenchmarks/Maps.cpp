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
#include <cassert>
#include <cmath>

using namespace PpFf;

static unsigned granularite = 2;

static int* mapFunc(int *in){
    int* res = (int*) malloc(sizeof(int));
    *res = *in;
    int nb = pow(10, granularite);
    for (int i = 1; i <= nb; i++) {
        *res += 1;
    }
    return res;
};

int main(int argc, char* argv[]) {
    // On definit les parametres d'execution.
    unsigned int n = 1000;
    unsigned int nbMaps = 2;
    unsigned int nbThreads = 0; // Execution sequentielle par defaut.
    
    int numArg = 1;
    if (argc > numArg) {
        n = atoi(argv[numArg]);
        assert( n > 0 );
    }
    numArg += 1;
    if (argc > numArg) {
        nbMaps = atoi(argv[numArg]);
        assert( nbMaps >= 1 );
    }
    numArg += 1;
    if (argc > numArg) {
        granularite = atoi(argv[numArg]);
        assert( granularite >= 0 );
    }
    numArg += 1;
    if (argc > numArg) {
        nbThreads = atoi(argv[numArg]);
        assert( nbThreads > 0 );
    }
    numArg += 1;

    // On definit les elements a traiter et le resultat attendu.
    std::vector<int> elems(n);
    std::vector<int> attendus(n);
    for (unsigned int i = 0; i < n; i++) {
        elems[i] = i;
        attendus[i] = nbMaps * pow(10, granularite) + i;
    }
    
    std::vector<int> obtenus;
    
    auto begin = std::chrono::high_resolution_clock::now();

        // EXECUTION PARALLELE.
    // On construit le pipe.

        auto pipe = 
            Flow
            ::source<int>(elems.begin(), elems.end())
				.parallel(nbThreads);
      
        for (unsigned i = 0; i < nbMaps; i++) {
            pipe = 
                pipe
                .map<int,int>(mapFunc);
        }

        obtenus = 
            pipe
            .collect<int, std::vector>();

  /*
        obtenus = 
            Flow 
            ::source<int>(elems.begin(), elems.end())
				.parallel(nbThreads)
				.map<int,int>(mapFunc)
				//.parallel(1)
            .collect<int, std::vector>();
*/

    auto end = std::chrono::high_resolution_clock::now();
    
    std::sort(obtenus.begin(), obtenus.end());

    if (obtenus.size() != attendus.size()) {
        fprintf( stderr, 
                 "Tailles pas ok: obtenus = %lu vs. attendus = %lu\n", 
                 obtenus.size(), attendus.size() );
    }
 
    for (unsigned int i = 0; i < attendus.size(); i++) {
        if (obtenus[i] != attendus[i]) {
            fprintf( stderr, 
                     "Pas ok pour %d: obtenus = %d vs. attendus = %d\n", 
                     i, obtenus[i], attendus[i] );
            break;
        }
    }
  

    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "%d %d %d %d : %ld\n", n, nbMaps, granularite, nbThreads, duration_ms );
 

   return 0;
}
