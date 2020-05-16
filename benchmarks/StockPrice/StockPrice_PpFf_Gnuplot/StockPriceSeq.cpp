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

int main(int argc, char* argv[]) {
    std::string inputFile = argc >= 2 ? argv[1] : DEFAULT_INPUT_FILE;
    bool emitOutput = argc >= 3 && atoi(argv[2]) == 1;

    // On exécute et on mesure le temps d'exécution.
    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, double> currentResult;

    std::ifstream file(inputFile);
    std::string* line = new std::string;
    while (std::getline(file, *line)) {
        OptionData* od = getOptionData(line);
        StockAndPrice* sp = calculateStockPrice(od);
        
        auto mapIt = currentResult.find(sp->StockName);
        if (mapIt != currentResult.end()) {
            mapIt->second = std::max(sp->StockPrice, mapIt->second);
        } else {
            currentResult[sp->StockName] = sp->StockPrice;
        }

        line = new std::string;
    }
        
    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();


    // On émet le temps d'exécution ou les résultats, selon le mode.
    if (!emitOutput) {
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
