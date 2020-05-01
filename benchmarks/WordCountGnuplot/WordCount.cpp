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

#define DEBUG_MODE false
#define DEFAULT_INPUT_FILE "/home/iuly/WorkplaceEclipse/PpFf-OldVersion/tests/WordCount/testdata/78792Words.txt"

#define DEFAULT_NB_THREADS 1
typedef std::vector<std::string> Words;

Words* splitInWords(std::string* line) {
    std::string delimiter = " ";

    Words* words = new Words();
    size_t start = 0, end = 0;
    do {
        end = line->find(delimiter, start);
        size_t len = end - start;
        words->push_back( line->substr(start, len) );
        start += len + delimiter.length();
    } while (end != std::string::npos);

    return words;
}

std::string* toLowercaseLetters(std::string* data) {
    std::string* result = new std::string;
    for (char c: *data) {
        if ('a' <= c && c <= 'z') {
            result->push_back(c);
        } else if ('A' <= c && c <= 'Z') {
            result->push_back(c-('Z'-'z'));
        }
    }
    return result;
}

bool notEmpty(std::string* s) {
    return s->size() > 0;
}

int main(int argc, char* argv[]) {
    bool debug = DEBUG_MODE;
    std::string inputFile = DEFAULT_INPUT_FILE;
    uint32_t nbThreads = DEFAULT_NB_THREADS;

    if (argc >= 2) {
        nbThreads = atoi(argv[1]);
    }

    if (argc >= 3) {
        inputFile = argv[2];
    }

    // utiliser pour vérifier le bon fonctionnement
    // du programme
    if (argc >= 4) {
        if(atoi(argv[3]) == 1){
	debug = true;
        }
    }

    Reducer<std::string, int> reducer(0, 
                                      [](int count, std::string _) { return count + 1; },
                                      std::plus<int>{} );

    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, int> currentResult;
    currentResult = 
        Flow
        ::source(inputFile)
        .parallel(nbThreads)
        .flatMap<std::string, std::string, Words>(splitInWords)			
        .map<std::string, std::string>(toLowercaseLetters)			
        .find<std::string>(notEmpty)	
        .reduceByKey<std::string, std::string, int>(reducer);  

    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    if(!debug){
        printf("%5ld ", duration_ms);
    } else{
        for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
            std::string currentResultKey = it->first;
            int currentResultValue = it->second;
            std::cout << currentResultKey << " => " << currentResultValue << std::endl;
        }
    } 

    return 0;
}
