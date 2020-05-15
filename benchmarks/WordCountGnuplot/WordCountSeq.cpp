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

static Words splitInWords(std::string line) {
    Words* words = new Words();
    std::string delimiter = " ";

    size_t start = 0, end = 0;
    do {
        end = line.find(delimiter, start);
        size_t len = end - start;
        words->push_back( line.substr(start, len) );
        start += len + delimiter.length();
    } while (end != std::string::npos);

    return *words;
}

static std::string toLowercaseLetters(std::string data) {
    std::string* result = new std::string;
    for (char c: data) {
        if ('a' <= c && c <= 'z') {
            result->push_back(c);
        } else if ('A' <= c && c <= 'Z') {
            result->push_back(c-('Z'-'z'));
        }
    }
    return *result;
}

int main(int argc, char *argv[]) {
    std::string inputFile = argc >= 2 ? argv[1] : DEFAULT_INPUT_FILE;

    // Utilisé pour vérifier le bon fonctionnement du programme
    bool emitOutput = argc >= 3 && atoi(argv[2]) == 1;

    // Crée et exécute le pipeline
    auto begin = std::chrono::high_resolution_clock::now();

    std::unordered_map<std::string, int> currentResult;
    currentResult.clear();
    std::ifstream file(inputFile);
    auto lines = std::vector<std::string*>();

    std::string line;
    while (std::getline(file, line)) {
        Words words = splitInWords(line);
        for (auto word = words.begin(); word != words.end(); word++) {
           std::string wordLC = toLowercaseLetters(*word);
           if ( wordLC.length() > 0 ) {
               auto theWord = currentResult.find(wordLC);
               if (theWord == currentResult.end()) {
                   currentResult.emplace(wordLC, 1);
               } else {
                   currentResult[theWord->first] = theWord->second + 1;
               }
            }
        }
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
