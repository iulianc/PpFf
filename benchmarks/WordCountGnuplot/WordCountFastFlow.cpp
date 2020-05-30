#include <iostream>
#include <ff/ff.hpp>
#include <functional>
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <locale>
#include <unordered_map>

using namespace ff;

#define DEFAULT_INPUT_FILE "testdata/loremipsum.txt"
#define DEFAULT_FARM_PARALLELISM 1

typedef std::vector<std::string> Words;

template <typename T>
std::string numberToString (T number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}


struct linesFromFileStage : ff_node {
    std::string const &path;
    linesFromFileStage(std::string const &path) : path(path){}
    
    void* svc(void*) {
        std::ifstream file(path);

        std::string* line = new std::string;
        while (std::getline(file, *line)) {
            ff_send_out(line);
            line = new std::string;
        }
        return EOS;
    }
};

#include "auxiliary-functions.hpp"

struct splitInWordsStage : ff_node_t<std::string, Words> {
    Words* svc(std::string* task) {
        return splitInWords(task);
    }
};

struct flatStage : ff_node_t<Words,std::string> {
    std::string* svc(Words* task) {
        for (auto &elem: *task) {
            ff_send_out(&elem);
        }
        return GO_ON;
    }
};

struct toLowercaseLettersStage : ff_node_t<std::string> {
    std::string* svc(std::string* task) {
        return toLowercaseLetters(task);
    }
};

struct groupByKeyStage : ff_node_t<std::string,void> {
    typedef std::unordered_map<std::string, int> CONTAINER;
    CONTAINER& container;

    groupByKeyStage(CONTAINER& container) : container(container){}

    void* svc(std::string* task) {
        container[*task] += 1;
        return GO_ON;
    }
};


int main(int argc, char *argv[]) {
    uint32_t nbFarmWorkers = argc >= 2 ? atoi(argv[1]) : DEFAULT_FARM_PARALLELISM;
    std::string inputFile = argc >= 3 ? argv[2] : DEFAULT_INPUT_FILE;

    // Utilisé pour vérifier le bon fonctionnement du programme
    bool emitOutput = argc >= 4 && atoi(argv[3]) == 1;

    // Crée et exécute le pipeline
    auto begin = std::chrono::high_resolution_clock::now();

    // Crée les workers pour le farm.
    std::vector<ff_node*> workers;
    for ( uint32_t i = 0; i < nbFarmWorkers; i++ ) {
        workers.push_back( new ff_Pipe<>( new splitInWordsStage,
                                          new flatStage,
                                          new toLowercaseLettersStage ) );
    }

    // Crée le pipeline dans son ensemble.
    std::unordered_map<std::string, int> result;

    ff_Pipe<> ffp( new linesFromFileStage(inputFile),
                   new ff_farm(workers),
                   new groupByKeyStage(result) );

    if (ffp.run_and_wait_end() < 0) 
        error("running pipe");

    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = 
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();


    // Affiche le résultat -- temps ou <<vrais résultats>>
    if (!emitOutput) {
        printf("%5ld ", duration_ms);
    } else {
        for (auto it = result.begin(); it != result.end(); it++) {
            std::string currentResultKey = it->first;
            int currentResultValue = it->second;

            std::cout 
                << currentResultKey 
                << " => " 
                << numberToString(currentResultValue) << std::endl;
        }	
    }

    return 0;
}






