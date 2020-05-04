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
#define DEFAULT_NB_THREADS 1
#define DEBUG_MODE false

struct StockAndPrice {
    std::string StockName;
    double StockPrice;
};

OptionData* getOptionData(std::string* data) {
    std::string name;
    OptionData* opt = new OptionData();
    char otype;
    std::stringstream ins(*data);

    /* read stock option data */
    ins >> opt->StockName;
    ins >> opt->s >> opt->strike >> opt->r >> opt->divq;
    ins >> opt->v >> opt->t >> otype >> opt->divs >> opt->DGrefval;
    opt->OptionType = (otype == 'P');
    
    return opt;
}

StockAndPrice* calculateStockPrice(OptionData* opt) {
    StockAndPrice* stockAndPrice = new StockAndPrice();
    stockAndPrice->StockName = opt->StockName;
    stockAndPrice->StockPrice = BlkSchlsEqEuroNoDiv(opt->s, opt->strike, opt->r, opt->v, 
                                                    opt->t, opt->OptionType, 0);
	
    return stockAndPrice;
}

int main(int argc, char* argv[]) {
	std::string inputFile = DEFAULT_INPUT_FILE;
	uint32_t nbThreads = DEFAULT_NB_THREADS;
	bool debug = DEBUG_MODE;

    if (argc >= 2) {
        inputFile = argv[1];
    }

    if (argc >= 3) {
        nbThreads = atoi(argv[2]);
    }

    // utiliser pour vérifier le bon fonctionnement
    // du programme
    if (argc >= 4) {
        if(atoi(argv[3]) == 1){
			debug = true;
        }
    }
    

    Reducer<StockAndPrice, double> reducer(0.0, 
                                           [](double maxPrice, StockAndPrice sp) {
                                               return sp.StockPrice > maxPrice ? sp.StockPrice : maxPrice; 
                                           },
                                           [](double max, double workerResult) { 
                                               return workerResult > max ? workerResult : max; 
                                           });
    
    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, double> currentResult;

    currentResult = 
        Flow
        ::source(inputFile)
        .parallel(nbThreads)
        .map<std::string, OptionData>(getOptionData)
        .map<OptionData, StockAndPrice>(calculateStockPrice)
        .reduceByKey<StockAndPrice, std::string, double>(reducer, 
                                                         [](StockAndPrice* sp) { return &(sp->StockName); });
        

    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    
	if(!debug){
    	printf("%5ld ", duration_ms);
	}else{
    	std::map<std::string, double> orderedResult;
    	for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
    		orderedResult[it->first] = it->second;
    	}
    
    	for (auto it = orderedResult.begin(); it != orderedResult.end(); it++) {
        	std::string currentResultKey = it->first;
        	double currentResultValue = it->second;
        	std::cout << currentResultKey << " => " << std::fixed << std::setprecision(4) << currentResultValue << std::endl;
    	}
	}
 
    return 0;
}
