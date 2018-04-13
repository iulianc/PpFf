#include <iostream>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <functional>
#include <regex>
#include <chrono>
#include <ctime>
#include <ratio>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include<fstream>
#include <locale>
#include <unordered_map>

using namespace ff;

int N = 10;


struct uptoStage: ff_node {
    int* vals = (int*) malloc(N * sizeof(int));

    void* svc(void* task) {
        for( int k = 0; k < N; k++ ) {
            vals[k] = k;
            ff_send_out(&vals[k]);
        }
        return EOS;
    }
};

struct fois10Stage: ff_node_t<int> {
    int* svc(int* task) {
        *task *= 10;

        return task;
    }
};

struct divise20Stage: ff_node_t<int> {
    int* svc(int* task) {
        return *task % 20 == 0 ? task : GO_ON;
    }
};

struct sommeJusquaStage: ff_node_t<int> {
    int* svc(int* task) {
        int res = 0;
        for (int i = 1; i <= *task; i++) {
            res += i / 20 % 2 == 0 ? 1 : 2;
        }

        *task = res;

        return task;
    }
};

std::vector<int> result;

struct collectStage:ff_node_t<int> {
    int* svc(int* task) {
        result.push_back(*task);

        return GO_ON;
    }
};



int main(int argc, char *argv[]) {

    if (argc >= 2) {
        N = atoi(argv[1]);
    }

    uptoStage upto;
    fois10Stage fois10;
    divise20Stage divise20;
    sommeJusquaStage sommeJusqua;
    collectStage collect;
    
    auto begin = std::chrono::high_resolution_clock::now();
    ff_pipeline ffp;
    ffp.add_stage(&upto);
    ffp.add_stage(&fois10);
    ffp.add_stage(&divise20);
    ffp.add_stage(&sommeJusqua);
    ffp.add_stage(&collect);
    
    if (ffp.run_and_wait_end()<0) error("running pipe");
    auto end = std::chrono::high_resolution_clock::now();
    long duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();
    printf( "Temps fastflow = %ld\n", duration_ms );

    /*
    printf( "\n" );
    printf( "*** Contenu de result ***\n" );
    for (auto it = result.begin(); it != result.end(); it++) {
        int currentResultKey = *it;
        std::cout << currentResultKey << std::endl;
    } 
    */
    
    return 0;
}
