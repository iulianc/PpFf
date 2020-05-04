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

#include "black_scholes.hpp"

using namespace ff;


#define DEFAULT_INPUT_FILE "../testdata/stock_options_64K.txt"
#define DEFAULT_NB_THREADS 1
#define DEBUG_MODE false


template <typename T>
std::string numberToString (T number) {
    std::ostringstream ss;
    ss << number;
    return ss.str();
}


struct LinesFromFileStage: ff_node {
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

OptionData* GetOptionData(std::string* data) {
    std::string name;
    OptionData* opt = new OptionData();
    char otype;
    std::stringstream ins(*data);

    ins >> opt->StockName;
    ins >> opt->s >> opt->strike >> opt->r >> opt->divq;
    ins >> opt->v >> opt->t >> otype >> opt->divs >> opt->DGrefval;
    opt->OptionType = (otype == 'P');
    
    return opt;
}

struct GetOptionDataStage: ff_node {

    void* svc(void* task) {
        std::string *line = (std::string*)task;
        return GetOptionData(line);
    }
};

struct StockAndPrice {
    std::string StockName;
    double StockPrice;
};

StockAndPrice* CalculateStockPrice(OptionData* opt) {
    StockAndPrice* stockAndPrice = new StockAndPrice();
    stockAndPrice->StockName = opt->StockName;
    stockAndPrice->StockPrice = BlkSchlsEqEuroNoDiv(opt->s, opt->strike, opt->r, opt->v, 
                                                    opt->t, opt->OptionType, 0);
	
    return stockAndPrice;
}

struct CalculateStockPriceStage: ff_node {

    void* svc(void* task) {
    	OptionData* opt = (OptionData*)task;
    	return CalculateStockPrice(opt);
    }
};

struct ReduceByKeyStage: ff_node {
	std::unordered_map<std::string, double> &result;

	ReduceByKeyStage(std::unordered_map<std::string, double> &result):result(result){}

	void* svc(void* task) {
		StockAndPrice* sp = (StockAndPrice*)task;
      auto mapIt = result.find(sp->StockName);

      if (mapIt != result.end()) {
      	mapIt->second = std::max(sp->StockPrice, mapIt->second);
      } else {
      	result[sp->StockName] = sp->StockPrice;
      }

		return GO_ON;
	}
};

struct CollectorStage:ff_node {
	typedef std::vector<StockAndPrice*> CONTAINER;
	CONTAINER &container;

	CollectorStage(CONTAINER &container): container(container){}

	void* svc(void* task) {
		container.push_back( (StockAndPrice*)task );
			return GO_ON;
    	}
};

struct Empty: public ff_node {
	public:
   	void* svc(void* task) {
    	return task;
    }
};



int main(int argc, char *argv[]) {
	std::string inputFile = DEFAULT_INPUT_FILE;
	uint32_t nbThreads = DEFAULT_NB_THREADS;
	std::unordered_map<std::string, double> result;
	bool debug = DEBUG_MODE;

    if (argc >= 2) {
        inputFile = argv[1];
    }

    if (argc >= 3) {
        nbThreads = atoi(argv[2]);
    }

    // utiliser pour v�rifier le bon fonctionnement
    // du programme
    if (argc >= 4) {
        if(atoi(argv[3]) == 1){
			debug = true;
        }
    }


	auto begin = std::chrono::high_resolution_clock::now();
	


	ff_pipeline ffp;
	ff_farm farm;
	std::vector<ff_node*> workers;


	LinesFromFileStage linesFromFile(inputFile);
	GetOptionDataStage getOptionData;
	CalculateStockPriceStage calculateStockPrice;
	ReduceByKeyStage reduceByKey(result);


	ffp.add_stage(&linesFromFile);

	if(nbThreads > 1){
		for(uint32_t i = 0; i < nbThreads; i++){
			ff_pipeline *p = new ff_pipeline();
			p->add_stage(new GetOptionDataStage());
			p->add_stage(new CalculateStockPriceStage());
			workers.push_back(p);
		}		

		farm.add_workers(workers);
		farm.add_collector(new Empty());

		ffp.add_stage(&farm);
		ffp.add_stage(new ReduceByKeyStage(result));

	}
	else {
		ffp.add_stage(&getOptionData);
		ffp.add_stage(&calculateStockPrice);
		ffp.add_stage(&reduceByKey);
	}

	if (ffp.run_and_wait_end()<0) error("running pipe");

	// Clear
	if(workers.size() > 0){
		for(uint32_t i = 0; i < nbThreads; i++){
			delete workers[i];
		}
		workers.clear();
	}


	auto end = std::chrono::high_resolution_clock::now();
	long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

	

	if(!debug){
    	printf("%5ld ", duration_ms);
	}
	else{
		std::map<std::string, double> resultFinal;

   		for (auto it = result.begin(); it != result.end(); it++) {
			resultFinal[it->first] = it->second;
			//std::cout << it->first << " => " << std::fixed << std::setprecision(4) << it->second << std::endl;
   		}

   		for (auto it = resultFinal.begin(); it != resultFinal.end(); it++) {
     		std::string currentResultKey = it->first;
      		double currentResultValue = it->second;
      		std::cout << currentResultKey << " => " << std::fixed << std::setprecision(4) << currentResultValue << std::endl;
   		}
	}

	return 0;
}






