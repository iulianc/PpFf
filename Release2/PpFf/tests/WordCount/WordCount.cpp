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

#define DEFAULT_NB_ITERATIONS 5
//#define DEFAULT_INPUT_FILE "testdata/78792Words.txt"
#define DEFAULT_INPUT_FILE "/home/iuly/WorkplaceEclipse/PpFf-OldVersion/tests/WordCount/testdata/78792Words.txt"

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

void incCount(int& count, std::string* _) {
    count += 1;
}

int main(int argc, char *argv[]) {
    uint32_t nbIterations = DEFAULT_NB_ITERATIONS;
    std::string inputFile = DEFAULT_INPUT_FILE;

    if (argc >= 2) {
        inputFile = argv[1];
    }

    if (argc >= 3) {
        nbIterations = atoi(argv[2]);
    }

    Reducing< std::string, int > reducing(0, [](int *count, std::string *word) ->void { *count += 1; },
     									  	 [](int *total, int *workerResult) ->void { *total += *workerResult; } );

    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, int> currentResult;
    for (uint32_t i = 0; i < nbIterations; ++i) {
        Pipe pipe;
        currentResult = pipe
            .linesFromFile(inputFile)
            .flatMap<std::string, std::string, Words>(splitInWords)
            .map<std::string, std::string>(toLowercaseLetters)
            .find<std::string>(notEmpty)
            .reduceByKey<std::string, std::string, int>(reducing);

    }
    auto end = std::chrono::high_resolution_clock::now();

    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    std::cerr << "Temps C++:  " << duration_ms / nbIterations << " ms" << std::endl;

    for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
        std::string currentResultKey = it->first;
        int currentResultValue = it->second;
        std::cout << currentResultKey << " => " << currentResultValue << std::endl;
    }

    return 0;
}







