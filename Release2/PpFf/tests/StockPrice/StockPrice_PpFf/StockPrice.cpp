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
#include <iomanip>

#include "black_scholes.hpp"

using namespace PpFf;

#define DEFAULT_NB_ITERATIONS 1
#define DEFAULT_INPUT_FILE "/home/iuly/PpFf_JapetServer/tests/StockPrice/testdata/stock_options_64K.txt"
#define DEFAULT_NB_THREADS 1
#define DEFAULT_OUTPUT_FILE_RESULT "/home/iuly/PpFf_JapetServer/tests/StockPrice/testdata/stock_output_result.txt"


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
    uint32_t nbIterations = DEFAULT_NB_ITERATIONS;
    std::string inputFile = DEFAULT_INPUT_FILE;
	 std::string outputFileResult = DEFAULT_OUTPUT_FILE_RESULT;
    uint32_t nbThreads = DEFAULT_NB_THREADS;

    if (argc >= 2) {
        inputFile = argv[1];
    }

    if (argc >= 3) {
        nbIterations = atoi(argv[2]);
    }
    
    if (argc >= 4) {
        nbThreads = atoi(argv[3]);
    }

    if (argc >= 5) {
        outputFileResult = argv[4];
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
    for (uint32_t i = 0; i < nbIterations; ++i) {
        currentResult = 
            Pipe()
            .linesFromFile(inputFile)
            .parallel(nbThreads)
            .map<std::string, OptionData>(getOptionData)
            .map<OptionData, StockAndPrice>(calculateStockPrice)
            //.parallel(1)
            .reduceByKey<StockAndPrice, std::string, double>(reducer, 
                                                             [](StockAndPrice* sp) { return &(sp->StockName); });
        
    }
    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    
    fprintf( stderr, "Temps C++  (%3d it.; %2d thr.): %5ld ms {%5ld ms/it. }\n",
             nbIterations, nbThreads, duration_ms, duration_ms / nbIterations );
    
  	 // Write result to file
	 std::ofstream ofs;

  	 ofs.open (outputFileResult, std::ofstream::out | std::ofstream::app);
  	 
	 char buffer[100];
	 snprintf(buffer, sizeof(buffer), "Temps C++  (%3d it.; %2d thr.): %5ld ms {%5ld ms/it. }\n", 
												nbIterations, nbThreads, duration_ms, duration_ms / nbIterations);
	 ofs << buffer;
	 ofs.close();

    std::map<std::string, double> orderedResult;
    for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
    	orderedResult[it->first] = it->second;
    }
    
    for (auto it = orderedResult.begin(); it != orderedResult.end(); it++) {
        std::string currentResultKey = it->first;
        double currentResultValue = it->second;
        std::cout << currentResultKey << " => " << std::fixed << std::setprecision(4) << currentResultValue << std::endl;
    }
   
    return 0;
}
