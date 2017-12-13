#include <iostream>
#include <vector>
#include "../../src/pp/Pipe.hpp"
#include <sstream>
#include <functional>
#include <regex>
#include <fstream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <string>
#include <ctype.h>

#define DEFAULT_NB_ITERATIONS 5

#define DEFAULT_INPUT_FILE "testdata/78792Words.txt"
//#define DEFAULT_INPUT_FILE "/home/iuly/RepositoryFastFlow/PpFf_Catch/tests/WordCount/testdata/loremipsum.txt"

template <typename T>
std::string numberToString (T number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

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

int main(int argc, char *argv[]) {
    uint32_t nbIterations = DEFAULT_NB_ITERATIONS;
    std::string inputFile = DEFAULT_INPUT_FILE;

    if (argc > 2) {
        nbIterations = atoi(argv[1]);
        inputFile = argv[2];
    }

    auto begin = std::chrono::high_resolution_clock::now();

    std::map<std::string, int> currentResult;
    for (uint32_t i = 0; i < nbIterations; ++i) {
        pp::Pipe pipe;
        currentResult = pipe
            .linesFromFile(inputFile)
            .flatMap<std::string, std::string, Words>(splitInWords)
            .map<std::string, std::string>( [](std::string* data) {
                    return new std::string(std::regex_replace(*data, std::regex("[^a-zA-Z]"), ""));
                })
            .find<std::string>( [](std::string *data) { return data->length() > 0; } )
            .map<std::string, std::string>( [](std::string *data) {
                    transform(data->begin(), data->end(), data->begin(), [](unsigned char c){ return std::tolower(c); });
                    return data;
                } )
            .groupByKey<std::string, std::string, int>( [](int& count, std::string* data) { count = count+1; } );

    }
    auto end = std::chrono::high_resolution_clock::now();

    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    std::cerr << "Temps C++:  " << duration_ms / nbIterations << " ms" << std::endl;
    
    for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
        std::string currentResultKey = it->first;
        int currentResultValue = it->second;
        std::cout << currentResultKey << " => " << numberToString(currentResultValue) << std::endl;
    }
    
    return 0;
}
