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
#define DEFAULT_OUTPUT_FILE_RESULT "/home/iuly/WorkplaceEclipse/PpFf-OldVersion/tests/WordCount/testdata/wordcount_benchmark_result.txt"
#define DEFAULT_NB_THREADS 1
#define DEFAULT_NB_WORDS 78792
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
    uint32_t nbIterations = DEFAULT_NB_ITERATIONS;
    std::string inputFile = DEFAULT_INPUT_FILE;
	 std::string outputFileResult = DEFAULT_OUTPUT_FILE_RESULT;
	 uint32_t nbThreads = DEFAULT_NB_THREADS;
	 uint32_t nbWords = DEFAULT_NB_WORDS;

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

    if (argc >= 6) {
        nbWords = atoi(argv[5]);
    }

    Reducer<std::string, int> reducer(0, 
                                      [](int count, std::string _) { return count + 1; },
                                      std::plus<int>{} );
    
    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, int> currentResult;
    for (uint32_t i = 0; i < nbIterations; ++i) {
        currentResult = 
            Pipe()
            .linesFromFile(inputFile)
				.parallel(nbThreads)
            .flatMap<std::string, std::string, Words>(splitInWords)			
            .map<std::string, std::string>(toLowercaseLetters)			
            .find<std::string>(notEmpty)	
            .reduceByKey<std::string, std::string, int>(reducer);

    }
    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    //std::cerr << "Temps C++:  " << duration_ms / nbIterations << " ms" << std::endl;
    fprintf( stderr, "Temps C++  (%3d it.; %2d thr.; %7d words): %5ld ms {%5ld ms/it. }\n",
             nbIterations, nbThreads, nbWords, duration_ms, duration_ms / nbIterations );

  	 // Write result to file
	 std::ofstream ofs;

  	 ofs.open (outputFileResult, std::ofstream::out | std::ofstream::app);
  	 
	 char buffer[100];
	 snprintf(buffer, sizeof(buffer), "Temps C++  (%3d it.; %2d thr.; %7d words): %5ld ms {%5ld ms/it. }\n", 
												nbIterations, nbThreads, nbWords, duration_ms, duration_ms / nbIterations);
	 ofs << buffer;
	 ofs.close();

    for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
        std::string currentResultKey = it->first;
        int currentResultValue = it->second;
        std::cout << currentResultKey << " => " << currentResultValue << std::endl;
    }

    return 0;
}







