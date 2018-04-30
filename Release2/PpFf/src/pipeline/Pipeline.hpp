
#include <ff/pipeline.hpp>
#include <stages/BaseStage.hpp>
#include <pipeline/Farm.hpp>
#include <operators/Empty.hpp>

using namespace ff;

namespace PpFf {

	enum NodeTypes { NodeTypeNode, NodeTypeFarm , NodeTypePipeline, Undefined };

    class Pipeline {
    public:
    	bool asPipeline = false;

        Pipeline(): no_workers(1), farm(NULL) {}

        ~Pipeline(){
            for (unsigned int i = 0; i < stages.size(); i++) {
                delete (stages[i]);
            }

            stages.clear();
        }

        template< typename T >
        void addStage(T *stage) {
            assert(stage->workers.size() == no_workers);

            // On ajoute le stage dans les stages locaux.
            stages.push_back(stage);

            // On l'ajoute aussi dans le pipeline ou la farm fast_flow.
            if (!isParallel()) {
            	if(currentNodeType == NodeTypeFarm){	//le dernier noeud ajoute dans le flux est un Farm
            		workers.clear();
            	}

                pipeline.add_stage(stage->workers[0]);
                currentNodeType = NodeTypeNode;
                currentWorkerType = Undefined;
            } else {
            	if(asPipeline){		//le type de parallelisme : pipeline
            		if(currentWorkerType != NodeTypePipeline){	//on genere les workers de type pipeline
            			for(unsigned int i = 0; i < no_workers; i++){
            				ff_pipeline *pipe = new ff_pipeline();
            				workers.push_back(pipe);
            			}
            		}

        			for(unsigned int i = 0; i < no_workers; i++){	//on ajoute les workers
        				ff_pipeline *pipe = (ff_pipeline*)workers[i];
        				pipe->add_stage((stage->workers)[i]);
        			}

        			if(currentWorkerType != NodeTypePipeline){	//on ajoute le Farm dans le flux
        				ff_farm<> *farm = new ff_farm<>();
                		farm->add_workers(workers);
                		farm->add_collector(new Empty());
                		pipeline.add_stage(farm);
        			}

            		currentWorkerType = NodeTypePipeline;

            	}else{		//le type de parallelisme : un Farm pour chaque noeud
            		if(currentWorkerType == NodeTypePipeline){ //le dernier noeud ajoute dans le flux est un Farm avec workers de type pipeline
                		workers.clear();
                	}

                    for(ff_node *worker : stage->workers){	//on ajoute les workers
                    	workers.push_back(worker);
                    }

            		ff_farm<> *farm = new ff_farm<>();	//on ajoute le Farm dans le flux
            		farm->add_workers(workers);
            		farm->add_collector(new Empty());
            		pipeline.add_stage(farm);

            		currentNodeType = NodeTypeFarm;
            		currentWorkerType = Undefined;
            		workers.clear();
            	}

//                allocateFarm(); // Lazy allocation.
//                for (unsigned int i = 0; i < no_workers; i++) {
//                    ff_pipeline *worker_pipe = (ff_pipeline*) farm->getWorker(i);
//                    worker_pipe->add_stage(stage->workers[i]);
//                }
            }
        }

        int nbWorkers() {
            return no_workers;
        }

        void setNbWorkers(int no_workers) {
//            if (this->no_workers != 1) {
//                std::cerr << "*** Avertissement setNbWorkers(" << no_workers << "): this->no_workers != 1 => nouvel appel ignore***\n";
//                return;
//            }

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
            if (pipeline.run_and_wait_end() < 0) error("running pipeline");
        }

    private:
        void allocateFarm() {
            if (farm != NULL) return;

            farm = new OldFarm(no_workers);
            pipeline.add_stage(farm->getFarm());
        }

    private:
        unsigned int no_workers;
        std::vector<IStage*> stages;
        ff_pipeline pipeline;
        OldFarm* farm;
        NodeTypes currentNodeType = Undefined;		//dernier noeud ajouter dans le flux
        NodeTypes currentWorkerType = Undefined;	//le type de worker: pipeline ou node
        std::vector<ff_node*> workers;				//utilise pour ajouter les workers dans le Farm
    };

}

