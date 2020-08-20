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
        }

        ~Pipeline() {
            for (unsigned int i = 0; i < stages.size(); i++) {
                delete (stages[i]);
            }

            for (unsigned int i = 0; i < nodes.size(); i++) {
                delete (nodes[i]);
            }

            stages.clear();
            nodes.clear();
        }

        void* svc(void * task) { return NULL; }  
        NodeType type() { return nodeType; }
        int nbWorkers() { return no_workers; }        

        Node* getCurrentNode() {
            if (nodes.size() > 0)
                return nodes[nodes.size() - 1];

            return NULL;
        }

        template< typename T >
        void addStage(T *stage) {
            assert(stage->operators.size() == no_workers);

            existeSource = existeSource || stage->isSource();

            if (!existeSource) {
                throw std::invalid_argument( "The pipeline must have a source" );
                return;
            }

            // On ajoute le stage dans les stages locaux.
            stages.push_back(stage);
            
            // On l'ajoute aussi dans le pipeline ou la farm fast_flow.
            Node *currentNode = getCurrentNode();
            if (!isParallel()) {
            	if (currentNode != NULL && currentNode->type() == FARM_NODE) {
                    Farm *farm = (Farm*) currentNode;
                    farm->addCollector(new Empty());                    
            	}
                nodes.push_back(stage->operators[0]);
            } else {
            	Farm *farm;
            	if (currentNode == NULL || currentNode->type() != FARM_NODE) {
                    farm = new Farm(no_workers);
                    nodes.push_back(farm);
            	} else {
                    farm = (Farm*) currentNode;
                    if (farm->nbWorkers() != no_workers) {
                        farm->addCollector(new Empty());
                        farm = new Farm(no_workers);
                        nodes.push_back(farm);
                    }
            	}

                farm->addStage(stage);
            }
        }

        ff_node* ff_node_() {
            ff_pipeline *pipe = new ff_pipeline();
            for (Node *node : nodes) {
                pipe->add_stage(node->ff_node_());
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

        void run() {
            ff_pipeline *pipe = (ff_pipeline*) ff_node_();
            if (pipe->run_and_wait_end() < 0) error("running pipeline");
        }

    private:
        unsigned int no_workers;
        std::vector<Stage*> stages;
        std::vector<Node*> nodes;
        bool existeSource = false;      
    };

}

#endif
