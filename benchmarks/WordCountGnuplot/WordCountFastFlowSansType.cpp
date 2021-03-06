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

#define DEFAULT_DEBUG_MODE false
#define DEFAULT_INPUT_FILE "testdata/loremipsum.txt"
#define DEFAULT_NB_FARM_WORKERS 1

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

struct dummyCollector : ff_node_t<std::string> {
    std::string* svc(std::string* task) {
        return task;
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
    bool debug = DEFAULT_DEBUG_MODE;
    std::string inputFile = DEFAULT_INPUT_FILE;
    std::unordered_map<std::string, int> result;
    uint32_t nbFarmWorkers = DEFAULT_NB_FARM_WORKERS;

    if (argc >= 2) {
        nbFarmWorkers = atoi(argv[1]);
    }

    if (argc >= 3) {
        inputFile = argv[2];
    }

    // Utilis� pour v�rifier le bon fonctionnement du programme
    if (argc >= 4 && atoi(argv[3]) == 1) {
        debug = true;
    }

    // Cr�e et ex�cute le pipeline
    auto begin = std::chrono::high_resolution_clock::now();

    ff_pipeline ffp;
    ffp.add_stage( new linesFromFileStage(inputFile) );
    std::vector<ff_node*> workers;
    for (uint32_t i = 0; i < nbFarmWorkers; i++) {
        ff_pipeline *p = new ff_pipeline();
        p->add_stage( new splitInWordsStage );
        p->add_stage( new flatStage );
        p->add_stage( new toLowercaseLettersStage );
        workers.push_back(p);
    }
    ff_farm farm;
    farm.add_workers(workers);
    farm.add_collector( new dummyCollector() );
    ffp.add_stage(&farm);
    ffp.add_stage(new groupByKeyStage(result));

    if (ffp.run_and_wait_end() < 0) 
        error("running pipe");

    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = 
        std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();


    // Affiche le r�sultat -- temps ou <<vrais r�sultats>>
    if (!debug) {
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






