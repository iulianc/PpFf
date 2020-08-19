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

    class Pipeline: public Node {
    public:
        Pipeline() : no_workers(1) {
            nodeType = PIPELINE_NODE;
            pipe = new ff_pipeline();
        }

        ~Pipeline() {
            for (unsigned int i = 0; i < stages.size(); i++) {
                delete (stages[i]);
            }
            stages.clear();
        }

        template< typename T >
        void addStage(T *stage) {
            assert(stage->operators.size() == no_workers);
            if (stage->isSource()) {
                hasSource = true;
            } else if (!hasSource) {
                throw std::invalid_argument( "The pipeline must have a source" );
                return;
            }

            // On ajoute le stage dans les stages locaux.
            stages.push_back(stage);
            
            // On l'ajoute aussi dans le pipeline ou la farm fast_flow.
            if (!isParallel()) {
            	if (currentNode != NULL && currentNode->type() == FARM_NODE) {
                    ff_farm *ffFarm = (ff_farm*) currentNode->getNode();
                    ffFarm->add_collector(new Empty());
                    addNode(ffFarm);
            	}
                addNode(stage->operators[0]);
                currentNode = NULL;
            } else {
            	Farm *farm;
            	if (currentNode == NULL || currentNode->type() != FARM_NODE) {
                    currentNode = farm = new Farm(no_workers);
            	} else {
                    farm = (Farm*) currentNode;
                    if (farm->nbWorkers() != no_workers) {
                        ff_farm *ffFarm = (ff_farm*) farm->getNode();
                        ffFarm->add_collector(new Empty());
                        addNode(ffFarm);
                        currentNode = farm = new Farm(no_workers);
                    }
            	}

                farm->addStage(stage);

            	if (stage->isFinal()) {
                    ff_farm *ffFarm = (ff_farm*)farm->getNode();
                    ffFarm->remove_collector();
                    addNode(ffFarm);
            	}
            }
        }

        void addNode(ff_node *ffNode) {
            pipe->add_stage(ffNode);
        }

        ff_node* getNode() {
            return pipe;
        }

        NodeType type(){
            return nodeType;
        }

        int nbWorkers() {
            return no_workers;
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

        void run() {
            if (pipe->run_and_wait_end() < 0) error("running pipeline");
        }

    private:
        ff_pipeline *pipe;
        unsigned int no_workers;
        std::vector<Stage*> stages;
        Node *currentNode = NULL;
        bool hasSource = false;      
    };

}

#endif
