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

typedef std::vector<std::string> vec_type;

template <typename T>
std::string NumberToString (T Number) {
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

int main(int argc, char *argv[]) {
    uint32_t nbIterations = 5;
    std::string inputFile = "testdata/78792Words.txt";
    //inputFile = "/home/iuly/RepositoryFastFlow/PpFf_Catch/tests/WordCount/testdata/loremipsum.txt";
    std::string outputFile;

    if (argc > 2) {
        nbIterations = atoi(argv[1]);
        inputFile = argv[2];
        std::cout << inputFile << std::endl;
                
        if (argc > 3) {
            outputFile = argv[3];
        }
        std::cout << outputFile << std::endl;
    }

    std::vector<std::string> words;
    std::ifstream file(inputFile);
    std::string line;
    while (std::getline(file, line)) {
        size_t start = 0;
        size_t end = 0;
        do { 
            std::string delimiter = " ";
            end = line.find(delimiter, start);
            size_t len = end - start;
            words.emplace_back( line.substr(start, len) );
            start += len + delimiter.length();
        } while (end != std::string::npos);
    }

    std::map<std::string, int> currentResult;
    auto begin = std::chrono::high_resolution_clock::now();
    
    for (uint32_t i = 0; i < nbIterations; ++i) {
        pp::Pipe pipe;
        currentResult = pipe
            .source<std::string>(words.begin(), words.end())
            .map<std::string, std::string>( [](std::string* data) {
                    std::string* result = new std::string;
                    for (auto& it: *data){
                        // JE NE COMPRENDS PAS CE BOUT DE CODE.
                        // Ca determine si c'est une lettre (code ASCII) => pas tres clair :(
                        // Mais cela couvre aussi d'autres caracteres on dirait!?
                        if ((((int) it) >= 65 && ((int) it) <= 90) || (((int) it) >= 95 && ((int) it) <= 122))
                            //if (('a' <= it && it <= 'z') || ('A' <= it && 'Z' <= it)) 
                            result->push_back(it);
                    }
                    return result;
                })
            .find<std::string>( [](std::string *data) { return data->length() > 0; } )
            .map<std::string, std::string>( [](std::string *data) {
                    transform(data->begin(), data->end(), data->begin(), [](unsigned char c){ return std::tolower(c); });
                    return data;
                } )
            .groupByKey<std::string, std::string, int>( [](int& count, std::string* data) { count = count+1; } );

    }
    auto end = std::chrono::high_resolution_clock::now();

    //long duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    //long duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    //std::cerr << "Duration: " << duration_ns / nbIterations << " ns" << std::endl;
    //std::cerr << "Duration: " << duration_us / nbIterations << " us" << std::endl;
    std::cerr << "Duration: " << duration_ms / nbIterations << " ms" << std::endl;

    std::fstream fs;
    if (!outputFile.empty()) {
        fs.open (outputFile, std::fstream::out);
    }

    for (auto it = currentResult.begin(); it != currentResult.end(); it++) {
        std::string currentResultKey = it->first;
        int currentResultValue = it->second;
        std::cout << currentResultKey << " => " << NumberToString(currentResultValue) << std::endl;
        if (!outputFile.empty()) {
            fs << currentResultKey << " => " << NumberToString(currentResultValue) << "\n";
        }
    }
        
    if (!outputFile.empty()) {
        fs.close();
    }

    return 0;
}
