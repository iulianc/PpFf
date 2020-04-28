#include <iostream>
#include <ff/ff.hpp>
#include <functional>
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>
#include<fstream>
#include <locale>
#include <unordered_map>
#include <ctype.h>
#include <cassert>
#include <cmath>


using namespace ff;

static unsigned granularity = 2;

class Worker1: public ff_node {
public:
    void* svc(void* task) {
		 printf("Worker1(%ld): TASK COMPUTED\n", get_my_id());
       return task;
    }
};

class Emitter1: public ff_node {
public:
    Emitter1(int ntasks):ntasks(ntasks) {}
    void* svc(void*) {
        for(long i=0;i<ntasks;++i)
            ff_send_out((void*)(i+10));
        return NULL;
    }
private:
    int ntasks;
};


struct Source: ff_node {
	std::vector<int> elems;

	Source(std::vector<int> elems): elems(elems){}

    void* svc(void* task) {
    	for (unsigned int i = 0; i < elems.size(); i++) {
			ff_send_out(new int(elems[i]));
		}

    	return EOS;
    }
};

static int* mapFunc(int *in){
    int* res = (int*) malloc(sizeof(int));
    *res = *in;
    int nb = pow(10, granularity);
    for (int i = 1; i <= nb; i++) {
        *res += 1;
    }
    return res;
};

struct Map: ff_node {

    void* svc(void* task) {
    	int* in = (int*)task;
    	return mapFunc(in);
    }
};

struct Collect: ff_node {
	std::vector<int> &result;

	Collect(std::vector<int> &result): result(result){}

    void* svc(void* task) {
    	int* in = (int*)task;
    	result.push_back(*in);

		return GO_ON;
    }
};

struct Empty: public ff_node {
	public:
   	void* svc(void* task) {
      	return task;
      }
};

int main(int argc, char* argv[]) {
	
   // On definit les parametres d'execution.
   unsigned int n = 1000;
   unsigned int nbMaps = 2;
   unsigned int nbThreads = 1; // Execution sequentielle par defaut.
    
   int numArg = 1;
   if (argc > numArg) {
       n = atoi(argv[numArg]);
       assert( n > 0 );
   }
   numArg += 1;
   if (argc > numArg) {
       nbMaps = atoi(argv[numArg]);
       assert( nbMaps >= 1 );
   }
   numArg += 1;
   if (argc > numArg) {
       granularity = atoi(argv[numArg]);
       assert( granularity >= 0 );
   }
   numArg += 1;
   if (argc > numArg) {
       nbThreads = atoi(argv[numArg]);
       assert( nbThreads > 0 );
   }
   numArg += 1;

   // On definit les elements a traiter et le resultat attendu.
   std::vector<int> elems(n);
   std::vector<int> attendus(n);
   for (unsigned int i = 0; i < n; i++) {
       elems[i] = i;
       attendus[i] = nbMaps * pow(10, granularity) + i;
   }
    
   std::vector<int> obtenus;
   
   auto begin = std::chrono::high_resolution_clock::now();

	ff_pipeline ffp;
	ff_farm farm;
	std::vector<ff_node*> workers;

	if(nbThreads > 1){
		ffp.add_stage(new Source(elems));
		
		for(uint32_t i = 0; i < nbThreads; i++){
			workers.push_back(new Map());
		}		

		farm.add_workers(workers);
		farm.add_collector(new Collect(obtenus));

		ffp.add_stage(&farm);

	}else {
		ffp.add_stage(new Source(elems));
		ffp.add_stage(new Map());
		ffp.add_stage(new Collect(obtenus));
	}
	
	if (ffp.run_and_wait_end()<0) error("running pipe");
	

    auto end = std::chrono::high_resolution_clock::now();
 
   
    std::sort(obtenus.begin(), obtenus.end());


    if (obtenus.size() != attendus.size()) {
        fprintf( stderr, 
                 "Tailles pas ok: obtenus = %lu vs. attendus = %lu\n", 
                 obtenus.size(), attendus.size() );
    }
 
    for (unsigned int i = 0; i < attendus.size(); i++) {
        if (obtenus[i] != attendus[i]) {
            fprintf( stderr, 
                     "Pas ok pour %d: obtenus = %d vs. attendus = %d\n", 
                     i, obtenus[i], attendus[i] );
            break;
        }
    }
  

    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "%d %d %d %d : %ld\n", n, nbMaps, granularity, nbThreads, duration_ms );
 

   return 0;
}
