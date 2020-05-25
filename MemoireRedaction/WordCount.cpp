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

#define DEFAULT_INPUT_FILE "testdata/78792Words.txt"
#define DEFAULT_FARM_PARALLELISM 1

typedef std::vector<std::string> Words;

#include "auxiliary-functions.hpp"

int main(int argc, char* argv[]) {
    uint32_t farmParallelism = argc >= 2 ? atoi(argv[1]) : DEFAULT_FARM_PARALLELISM;
    std::string inputFile = argc >= 3 ? argv[2] : DEFAULT_INPUT_FILE;

    // Utilise pour verifier le bon fonctionnement du programme
    bool emitOutput = argc >= 4 && atoi(argv[3]) == 1;
    
    auto begin = std::chrono::high_resolution_clock::now();

    Reducer<std::string, int> reducer(0, 
                                      [](int count, std::string _) { return count + 1; },
                                      std::plus<int>{} );

    std::unordered_map<std::string, int> currentResult = 
        Flow
        ::source(inputFile)
        .parallel(farmParallelism)
        .flatMap<std::string, std::string, Words>(splitInWords)			
        .map<std::string, std::string>(toLowercaseLetters)			
        .find<std::string>(notEmpty)	
        .reduceByKey<std::string, std::string, int>(reducer);  

    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = 
        std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    if (!emitOutput) {
        printf("%5ld ", duration_ms);
    } else {
        for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
            std::string currentResultKey = it->first;
            int currentResultValue = it->second;
            std::cout << currentResultKey << " => " << currentResultValue << std::endl;
        }
    } 

    return 0;
}
