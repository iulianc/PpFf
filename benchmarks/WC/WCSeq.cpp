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
typedef std::tuple<int,int,long long> Triple;

#include "auxiliary-functions.hpp"

int main(int argc, char *argv[]) {
    std::string inputFile = argc >= 2 ? argv[1] : DEFAULT_INPUT_FILE;
    
    // Utilisé pour vérifier le bon fonctionnement du programme
    bool emitOutput = argc >= 3 && atoi(argv[2]) == 1;

    // Crée et exécute le pipeline
    auto begin = std::chrono::high_resolution_clock::now();

    Triple result(0, 0, 0);

    std::ifstream file(inputFile);
    std::string* line = new std::string;
    while (std::getline(file, *line)) {
        Words* words = splitInWords(line);

        for (auto word = words->begin(); word != words->end(); word++) {
            std::string* wordLC = toLowercaseLetters(&(*word));
            if ( notEmpty(wordLC) ) {
                long long h = compute_hash(wordLC);
                std::get<0>(result) += 1;
                std::get<1>(result) += wordLC->size();
                if ( h > std::get<2>(result) ) {
                    std::get<2>(result) = h;
                }
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
        std::cout
            << std::get<0>(result)
            << " "
            << std::get<1>(result)
            << " "
            << std::get<2>(result)
            << " "
            << "\n";
    }

    return 0;
}
