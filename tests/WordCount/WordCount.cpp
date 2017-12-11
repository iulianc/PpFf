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

template <typename T>
std::string NumberToString (T Number) {
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

int main(int argc, char *argv[]) {
    std::string inputFile;
    std::string outputFile;
    std::fstream fs;
    uint32_t nbIterations = 5;

    if (argc > 2) {
        nbIterations = atoi(argv[1]);
        inputFile = argv[2];
        std::cout << inputFile << std::endl;
                
        if (argc > 3) {
            outputFile = argv[3];
        }
        std::cout << outputFile << std::endl;
    } else {
        //inputFile = "/home/iuly/RepositoryFastFlow/PpFf_Catch/tests/WordCount/testdata/loremipsum.txt";
        inputFile = "testdata/78792Words.txt";
    }

    std::vector<std::string> words;
    std::ifstream file(inputFile);
    std::string line;
    while (std::getline(file, line)) {
        size_t start = 0;
        size_t end = 0;
        size_t len = 0;
                
        do { 
            std::string delimiter = " ";
            end = line.find(delimiter, start);
            len = end - start;
            words.emplace_back( line.substr(start, len) );
            start += len + delimiter.length();
        } while (end != std::string::npos);
    }


    std::vector<std::vector<std::string>> container = {words};
    std::map<std::string, int> currentResult;

    typedef std::vector<std::string> vec_type;

    auto begin = std::chrono::high_resolution_clock::now();

    for (uint32_t i = 0; i < nbIterations; ++i) {
        pp::Pipe pipe;
        currentResult = pipe.source<vec_type>(container.begin(), container.end())
            .flatMap<vec_type, std::string>()
            .map<std::string, std::string>( [](std::string *data) {
                    std::string *result = new std::string;
                    for (auto& it: *data){
                        if ((((int) it) > 64 && ((int) it) < 91) || (((int) it) > 96 && ((int) it) < 123))
                            result->push_back(it);
                    }
                    return result;
                })
            .find<std::string>( [](std::string *data) { return data->length() > 0; } )
            .map<std::string, std::string>( [](std::string *data) {
                    transform(data->begin(), data->end(), data->begin(), [](unsigned char c){ return std::tolower(c); });
                    return data;
                } )
            .groupByKey<std::string, std::string, int>( [](int &count, std::string *data) { count = count+1; } );

    }

    auto end = std::chrono::high_resolution_clock::now();
    long duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    long duration_us = std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    std::cout << "Duration nanoseconds: " << duration_ns / nbIterations << "ns." << std::endl;
    std::cout << "Duration microseconds: " << duration_us / nbIterations << "us." << std::endl;
    std::cout << "Duration milliseconds: " << duration_ms / nbIterations << "ms." << std::endl;


    if (!outputFile.empty()) {
        fs.open (outputFile, std::fstream::out);
    }

    std::map<std::string, int>::iterator it = currentResult.begin();
    while (it != currentResult.end()) {
        std::string currentResultKey = it->first;
        int currentResultValue = it->second;
        //		std::cout << currentResultKey << " => " << NumberToString(currentResultValue) << std::endl;
        if (!outputFile.empty()) {
            fs << currentResultKey << " => " << NumberToString(currentResultValue) << "\n";
        }
        it++;
    }
        
    if (!outputFile.empty()) {
        fs.close();
    }

    return 0;
}
