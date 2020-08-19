#ifndef FARM_HPP
#define FARM_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>
#include <pipeline/Worker.hpp>
#include <stages/BaseStage.hpp>

using namespace ff;

namespace PpFf {

    class Farm: public Node {
    public:
        Farm(unsigned int no_workers): no_workers(no_workers) {
            nodeType = FARM_NODE;
            farm = new ff_farm();
            for (unsigned int i = 0 ; i < no_workers; i++) {
                workers.push_back(new Worker());
            }
        }

        template< typename T >
        void addStage(T *stage) {
            std::vector<ff_node*> nodes;
            nodes.assign(stage->operators.begin(), stage->operators.end());
            addNode(nodes);          
        } 

        NodeType type(){
            return nodeType;
        }

        unsigned int nbWorkers() {
            return no_workers;
        }

        void addNode(std::vector< ff_node* > stageWorkers) {
            assert(workers.size() == stageWorkers.size());

            for (unsigned int i = 0; i < workers.size(); i++){
                workers[i]->addNode(stageWorkers[i]);
            }
        }

        ff_node* getNode() {
            std::vector< ff_node* > innerWorkers;
            for (Worker* worker: workers) {
                innerWorkers.push_back((ff_node*) worker->getNode());
            }
            farm->add_workers(innerWorkers);

            return farm;
        }


    private:
        unsigned int no_workers;
        std::vector<Worker*> workers;
        ff_farm *farm;
    };
}

#endif
