#ifndef PIPMANAGER_HPP
#define PIPMANAGER_HPP

//#include <ff/pipeline.hpp>
#include <ff/ff.hpp>
#include <stages/BaseStage.hpp>
#include <pipeline/Farm.hpp>
#include <operators/Empty.hpp>
#include <pipeline/Node.hpp>
#include <pipeline/Worker.hpp>
#include <pipeline/Pipeline.hpp>
#include <stdexcept>

using namespace ff;

namespace PpFf {

    class PipeManager {
    public:
    	bool asPipeline = false;

        PipeManager(): no_workers(1) {}

        ~PipeManager(){
            for (unsigned int i = 0; i < stages.size(); i++) {
                delete (stages[i]);
            }
            stages.clear();
        }

        template< typename T >
        void addStage(T *stage) {
            assert(stage->workers.size() == no_workers);
            if(stage->isSource()) {
                hasSource = true;
            } else if(hasSource == false){
                throw std::invalid_argument( "The pipeline must have a source" );
                return;
            }

            // On ajoute le stage dans les stages locaux.
            stages.push_back(stage);

            // On l'ajoute aussi dans le pipeline ou la farm fast_flow.
            if (!isParallel()) {
            	if (currentNode != NULL && currentNode->Type() == NodeTypeFarm) {
                    ff_farm *ffFarm = (ff_farm*) currentNode->getNode();
                    ffFarm->add_collector(new Empty());
                    pipeline.addStage(ffFarm);
            	}
                pipeline.addStage(stage->workers[0]);
                currentNode = NULL;
            } else {
            	Farm *farm;
            	if (currentNode == NULL || currentNode->Type() != NodeTypeFarm) {
                    currentNode = farm = new Farm(no_workers);
            	} else {
                    farm = (Farm*) currentNode;
                    if (farm->nbWorkers() != no_workers) {
                        ff_farm *ffFarm = (ff_farm*) farm->getNode();
                        ffFarm->add_collector(new Empty());
                        pipeline.addStage(ffFarm);
                        currentNode = farm = new Farm(no_workers);
                    }
            	}
                
            	std::vector<ff_node*> stages;
                stages.assign(stage->workers.begin(), stage->workers.end());
            	farm->addStage(stages);

            	if (stage->isFinal()) {
                    ff_farm *ffFarm = (ff_farm*)farm->getNode();
                    ffFarm->remove_collector();
                    pipeline.addStage(ffFarm);
            	}
            }
        }

        int nbWorkers() {
            return no_workers;
        }

        void setNbWorkers(int no_workers) {
            if (this->no_workers < 1) {
                std::cerr << "*** Avertissement le nombre de workers ne peut pas être négative ***\n";
                return;
            }

            this->no_workers = no_workers;
        }

        bool isParallel() {
            return no_workers > 1;
        }

        void run() {
            pipeline.run();
        }

    private:
        unsigned int no_workers;
        std::vector<Stage*> stages;
        Node *currentNode = NULL;
        bool hasSource = false;
        Pipeline pipeline;
    };

}

#endif
