#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>
#include <pipeline/Farm.hpp>
#include <operators/Empty.hpp>
#include <pipeline/Worker.hpp>
#include <stdexcept>
#include <pipeline/Collector.hpp>


namespace PpFf {

    class Pipeline {
    public:
        Pipeline() : no_workers(1)
        {}

        ~Pipeline() {
            for (unsigned int i = 0; i < myNodes.size(); i++) {
                delete myNodes[i];
            }
            
            myNodes.clear();
        }

        int nbWorkers() {
            return no_workers;
        }

        template< typename TOperator, typename T >
        T value() {
            Collector<TOperator> collector(lastNodes);
            return collector.value();
        }        

        void addNodes(std::vector<Node*> nodes) {
            assert(nodes.size() == no_workers);

            sourceExists |= nodes[0]->isSource();

            if (!sourceExists) {
                throw std::invalid_argument( "The pipeline must have a source" );
                return;
            }

            if(nodes[0]->isCollector()){
                lastNodes = nodes;
            }
            
            // On l'ajoute aussi dans le pipeline ou la farm fast_flow.
            Node *lastNode = myNodes.size() > 0 ? myNodes.back() : NULL;
            Farm* farm = dynamic_cast<Farm*>(lastNode);

            if ( farm == NULL ) {
                // Le dernier noeud n'est pas un Farm.
                if (isParallel()) {
                    // Execution parallele => on cree un farm.
                    farm = new Farm(no_workers);
                    myNodes.push_back(farm);
                    farm->addNodes(nodes);
                } else {
                    // Execution non parallele => pas besoin de farm.
                    myNodes.push_back(nodes[0]);
                }
            } else {
                // Le dernier noeud est deja un Farm
                if (isParallel()) {
                    if (farm->nbWorkers() != no_workers) {
                        // Le nombre de workers a change => nouveau farm.
                        farm->addCollector(new Empty());
                        farm = new Farm(no_workers);
                        myNodes.push_back(farm);
                    }
                    farm->addNodes(nodes);
                } else {
                    // Redevenu sequentiel.
                    farm->addCollector(new Empty());                    
                    myNodes.push_back(nodes[0]);
                }
            }
        }

        ff_node* build_ff_node() {
            ff::ff_pipeline *pipe = new ff::ff_pipeline();
            for (Node *node : myNodes) {
                pipe->add_stage(node->build_ff_node());
            }

            return pipe;
        }

        void setNbWorkers(int no_workers) {
            if (this->no_workers < 1) {
                std::cerr << "*** Avertissement le nombre de workers ne peut pas etre nul ou negatif ***\n";
                return;
            }

            this->no_workers = no_workers;
        }

        bool isParallel() {
            return no_workers > 1;
        }        

        int run(bool = false) {
            ff::ff_pipeline *pipe = (ff::ff_pipeline*) build_ff_node();
            if (pipe->run_and_wait_end() < 0) error("running pipeline");

            return 1;
        }

    private:
        unsigned int no_workers;
        std::vector<Node*> myNodes;
        std::vector<Node*> lastNodes;
        bool sourceExists = false;
    };

}

#endif
