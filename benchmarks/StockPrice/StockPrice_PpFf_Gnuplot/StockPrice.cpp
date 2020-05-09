#include <iostream>
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
#include <fstream>
#include <locale>
#include <Flow.hpp>
#include <unordered_map>
#include <ctype.h>
#include <iomanip>

#include "black_scholes.hpp"

using namespace PpFf;

#define DEFAULT_INPUT_FILE "/home/iuly/PpFf_JapetServer/tests/StockPrice/testdata/stock_options_64K.txt"
#define DEFAULT_NB_FARM_WORKERS 1
#define DEFAULT_DEBUG false


int main(int argc, char* argv[]) {
    // On définit les paramètres d'exécution.
    std::string inputFile = DEFAULT_INPUT_FILE;
    uint32_t nbFarmWorkers = DEFAULT_NB_FARM_WORKERS;
    bool debug = DEFAULT_DEBUG;

    if (argc >= 2) {
        inputFile = argv[1];
    }

    if (argc >= 3) {
        nbFarmWorkers = atoi(argv[2]);
    }

    // Utilisé pour vérifier le bon fonctionnement du programme.
    if (argc >= 4 && atoi(argv[3]) == 1) {
        debug = true;
    }

    // On exécute et on mesure le temps d'exécution.
    auto begin = std::chrono::high_resolution_clock::now();

    Reducer<StockAndPrice, double> reducer(0.0, 
                                           [](double maxPrice, StockAndPrice sp) {
                                               return sp.StockPrice > maxPrice ? sp.StockPrice : maxPrice; 
                                           },
                                           [](double max, double workerResult) { 
                                               return workerResult > max ? workerResult : max; 
                                           });
    std::unordered_map<std::string, double> currentResult =
        Flow
        ::source(inputFile)
        .parallel(nbFarmWorkers)
        .map<std::string, OptionData>(getOptionData)
        .map<OptionData, StockAndPrice>(calculateStockPrice)
        .reduceByKey<StockAndPrice, std::string, double>(reducer, 
                                                         [](StockAndPrice* sp) { return &(sp->StockName); });
        
    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

    // On émet le temps d'exécution ou les résultats, selon le mode.
    if (!debug) {
    	printf("%5ld ", duration_ms);
    } else {
    	std::map<std::string, double> orderedResult;
    	for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
            orderedResult[it->first] = it->second;
    	}
    
    	for (auto it = orderedResult.begin(); it != orderedResult.end(); it++) {
            std::string currentResultKey = it->first;
            double currentResultValue = it->second;
            std::cout << currentResultKey
                      << " => "
                      << std::fixed
                      << std::setprecision(4)
                      << currentResultValue
                      << std::endl;
    	}
    }
 
    return 0;
}
