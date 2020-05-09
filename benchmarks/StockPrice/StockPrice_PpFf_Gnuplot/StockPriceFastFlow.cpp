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
#include <map>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <ctype.h>

using namespace ff;

#include "black_scholes.hpp"

#define DEFAULT_INPUT_FILE "../testdata/stock_options_64K.txt"
#define DEFAULT_NB_FARM_WORKERS 1
#define DEFAULT_DEBUG false


template <typename T>
std::string numberToString (T number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
};

struct LinesFromFileStage : ff_node {
    std::string const &path;

    LinesFromFileStage(std::string const &path):path(path){}

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

struct GetOptionDataStage : ff_node_t<std::string,OptionData> {
    OptionData* svc(std::string* task) {
        std::string *line = task;
        return getOptionData(line);
    }
};

struct CalculateStockPriceStage : ff_node_t<OptionData,StockAndPrice> {
    StockAndPrice* svc(OptionData* od) {
    	return calculateStockPrice(od);
    }
};

struct ReduceByKeyStage : ff_node_t<StockAndPrice,void> {
    std::unordered_map<std::string, double> &result;

    ReduceByKeyStage(std::unordered_map<std::string, double> &result) : result(result){}

    void* svc(StockAndPrice* sp) {
        auto mapIt = result.find(sp->StockName);

        if (mapIt != result.end()) {
            mapIt->second = std::max(sp->StockPrice, mapIt->second);
        } else {
            result[sp->StockName] = sp->StockPrice;
        }

        return GO_ON;
    }
};

struct CollectorStage : ff_node {
    typedef std::vector<StockAndPrice*> CONTAINER;
    CONTAINER &container;

    CollectorStage(CONTAINER &container): container(container){}

    void* svc(void* task) {
        container.push_back( (StockAndPrice*)task );
        return GO_ON;
    }
};

int main(int argc, char *argv[]) {
    std::string inputFile = DEFAULT_INPUT_FILE;
    uint32_t nbFarmWorkers = DEFAULT_NB_FARM_WORKERS;
    bool debug = DEFAULT_DEBUG;

    if (argc >= 2) {
        nbFarmWorkers = atoi(argv[1]);
    }

    if (argc >= 3) {
        inputFile = argv[2];
    }

    // Utilisé pour vérifier le bon fonctionnement du programme.
    if (argc >= 4 && atoi(argv[3]) == 1) {
        debug = true;
    }

    // Crée et exécute le pipeline
    auto begin = std::chrono::high_resolution_clock::now();

    // Crée les workers pour le farm.
    std::vector<ff_node*> workers;
    for (uint32_t i = 0; i < nbFarmWorkers; i++) {
        workers.push_back(  new ff_Pipe<>( new GetOptionDataStage(),
                                           new CalculateStockPriceStage() ) );
    }		

    // Crée le pipeline dans son ensemble.
    std::unordered_map<std::string, double> result;

    ff_Pipe<> ffp( new LinesFromFileStage(inputFile),
                   new ff_farm(workers),
                   new ReduceByKeyStage(result) );

    if (ffp.run_and_wait_end()<0)
        error("running pipe");

    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

    if (!debug) {
    	printf("%5ld ", duration_ms);
    } else {
        std::map<std::string, double> orderedResult;

        for (auto it = result.begin(); it != result.end(); it++) {
            orderedResult[it->first] = it->second;
        }

        for (auto it = orderedResult.begin(); it != orderedResult.end(); it++) {
            std::string currentResultKey = it->first;
            double currentResultValue = it->second;

            std::cout << currentResultKey
                      << " => "
                      << std::fixed
                      << std::setprecision(4)
                      << currentResultValue
                      << std::endl;
        }
    }

    return 0;
}






