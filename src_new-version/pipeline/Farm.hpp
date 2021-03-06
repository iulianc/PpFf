#ifndef FARM_HPP
#define FARM_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>
#include <pipeline/Worker.hpp>

namespace PpFf {

    class Farm: public Node {
    public:
        Farm(unsigned int no_workers) : no_workers(no_workers) {
            _ff_farm = new ff::ff_farm();
            for (unsigned int i = 0 ; i < no_workers; i++) {
                workers.push_back(new Worker());
            }
        }

        ~Farm() {
            for (unsigned int i = 0; i < workers.size(); i++) {
                delete workers[i];
            }

            workers.clear();
        }        

        void addNodes(std::vector<Node*> nodes) {
            if (nodes[0]->isCollector()) {
                _ff_farm->remove_collector();
            }

            assert(workers.size() == nodes.size());

            for (unsigned int i = 0; i < workers.size(); i++){
                workers[i]->addNode(nodes[i]);
            }
        } 

        unsigned int nbWorkers() {
            return no_workers;
        }

        void addCollector(ff_node* node) {
            _ff_farm->add_collector(node);
        }

        ff_node* build_ff_node() {
            std::vector<ff_node*> innerWorkers;
            for (Worker* worker: workers) {
                innerWorkers.push_back(worker->build_ff_node());
            }
            _ff_farm->add_workers(innerWorkers);

            return _ff_farm;
        }

    private:
        unsigned int no_workers;
        std::vector<Worker*> workers;
        ff::ff_farm* _ff_farm;
    };
}

#endif
