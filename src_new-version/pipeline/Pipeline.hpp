#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>
#include <stages/BaseStage.hpp>
#include <pipeline/Farm.hpp>
#include <operators/Empty.hpp>
#include <pipeline/Worker.hpp>
#include <stdexcept>

using namespace ff;

namespace PpFf {

    class Pipeline {
    public:
        Pipeline() : no_workers(1) {
        }

        ~Pipeline() {
            for (unsigned int i = 0; i < stages.size(); i++) {
                delete stages[i];
            }

            for (unsigned int i = 0; i < nodes.size(); i++) {
                delete nodes[i];
            }
            
            stages.clear();
            nodes.clear();
        }

        int nbWorkers() {
            return no_workers;
        }

        template< typename T >
        void addStage(T *stage) {
            assert(stage->operators.size() == no_workers);

            sourceExists |= stage->isSource();

            if (!sourceExists) {
                throw std::invalid_argument( "The pipeline must have a source" );
                return;
            }

            // On ajoute le stage dans les stages locaux.
            stages.push_back(stage);
            
            // On l'ajoute aussi dans le pipeline ou la farm fast_flow.
            Node *lastNode = nodes.size() > 0 ? nodes.back() : NULL;
            Farm* farm = dynamic_cast<Farm*>(lastNode);

            if ( farm == NULL ) {
                // Le dernier noeud n'est pas un Farm.
                if (isParallel()) {
                    // Execution parallele => on cree un farm.
                    farm = new Farm(no_workers);
                    nodes.push_back(farm);
                    farm->addStage(stage);
                } else {
                    // Execution non parallele => pas besoin de farm.
                    nodes.push_back(stage->operators[0]);
                }
            } else {
                // Le dernier noeud est deja un Farm
                if (isParallel()) {
                    if (farm->nbWorkers() != no_workers) {
                        // Le nombre de workers a change => nouveau farm.
                        farm->addCollector(new Empty());
                        farm = new Farm(no_workers);
                        nodes.push_back(farm);
                    }
                    farm->addStage(stage);
                } else {
                    // Redevenu sequentiel.
                    farm->addCollector(new Empty());                    
                    nodes.push_back(stage->operators[0]);
                }
            }
        }

        ff_node* build_ff_node() {
            ff_pipeline *pipe = new ff_pipeline();
            for (Node *node : nodes) {
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
            ff_pipeline *pipe = (ff_pipeline*) build_ff_node();
            if (pipe->run_and_wait_end() < 0) error("running pipeline");

            return 1;
        }

    private:
        unsigned int no_workers;
        std::vector<Stage*> stages;
        std::vector<Node*> nodes;
        bool sourceExists = false;      
    };

}

#endif
