#include <iostream>
#include <vector>
#include <sstream>
#include <functional>
#include <regex>
#include <fstream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <string>
#include <ctype.h>
#include <unordered_map> 

#define DEFAULT_INPUT_FILE "testdata/78792Words.txt"

typedef std::vector<std::string> Words;

#include "auxiliary-functions.hpp"

int main(int argc, char *argv[]) {
    std::string inputFile = argc >= 2 ? argv[1] : DEFAULT_INPUT_FILE;
    
    // Utilisé pour vérifier le bon fonctionnement du programme
    bool emitOutput = argc >= 3 && atoi(argv[2]) == 1;

    // Crée et exécute le pipeline
    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, size_t> currentResult;

    std::ifstream file(inputFile);
    std::string* line = new std::string;
    while (std::getline(file, *line)) {
        Words* words = splitInWords(line);
        for (auto word = words->begin(); word != words->end(); word++) {
            std::string* wordLC = toLowercaseLetters(&(*word));
            if ( notEmpty(wordLC) ) {
                currentResult[*wordLC] += 1;
            }
        }
        line = new std::string;
    }

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
