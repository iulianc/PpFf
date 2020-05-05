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
#define DEFAULT_NB_THREADS 1

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

    void* svc(void* task) {
        std::ifstream file(path);

        std::string* line = new std::string;
        while (std::getline(file, *line)) {
            ff_send_out(line);
            line = new std::string;
        }
        return EOS;
    }
};

struct splitInWordsStage : ff_node {
    std::string delimiter = " ";

    void* svc(void* task) {
    	std::string line = *((std::string*)task);

        Words* words = new Words();
        size_t start = 0, end = 0;
        do {
            end = line.find(delimiter, start);
            size_t len = end - start;
            words->push_back( line.substr(start, len) );
            start += len + delimiter.length();
        } while (end != std::string::npos);

        return words;
    }
};

struct flatStage : ff_node {
    std::string delimiter = " ";

    void* svc(void* task) {
        for(auto &elem: *(Words*)task){
            ff_send_out(&elem);
        }
        return GO_ON;
    }
};

struct toLowercaseLettersStage : ff_node_t<std::string> {
    std::string* svc(std::string* task) {
    	std::string* result = new std::string;

        for (auto& c: *task) {
            int ci = (int) c;
            if ('a' <= ci && ci <= 'z') {
                result->push_back(ci);
            } else if ('A' <= ci && ci <= 'Z') {
                result->push_back(c-('Z'-'z'));
            }
        }
        return result;
    }
};

struct filterEmptyWordsStage : ff_node_t<std::string> {
    std::string* svc(std::string* task) {
        if (task->size() > 0) {
            return task;
        }
        return GO_ON;
    }
};

struct dummyCollector : ff_node {
    void* svc(void* task) {
        return task;
    }
};

struct groupByKeyStage : ff_node {
    typedef std::unordered_map< std::string, int > CONTAINER;
    CONTAINER &container;

    groupByKeyStage(CONTAINER &container) : container(container){}

    void* svc(void* task) {
        container[*((std::string*)task)] += 1;
        return GO_ON;
    }
};


int main(int argc, char *argv[]) {
    bool debug = DEFAULT_DEBUG_MODE;
    std::string inputFile = DEFAULT_INPUT_FILE;
    std::unordered_map<std::string, int> result;
    uint32_t nbThreads = DEFAULT_NB_THREADS;

    if (argc >= 2) {
        nbThreads = atoi(argv[1]);
    }

    if (argc >= 3) {
        inputFile = argv[2];
    }

    // Utilisé pour vérifier le bon fonctionnement du programme
    if (argc >= 4 && atoi(argv[3]) == 1) {
        debug = true;
    }

    // Crée et exécute le pipeline
    auto begin = std::chrono::high_resolution_clock::now();

    ff_pipeline ffp;
    ffp.add_stage( new linesFromFileStage(inputFile) );
    std::vector<ff_node*> workers;
    for (uint32_t i = 0; i < nbThreads; i++) {
        ff_pipeline *p = new ff_pipeline();
        p->add_stage( new splitInWordsStage );
        p->add_stage( new flatStage );
        p->add_stage( new toLowercaseLettersStage );
        p->add_stage( new filterEmptyWordsStage );
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


    // Affiche le résultat -- temps ou <<vrais résultats>>
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






