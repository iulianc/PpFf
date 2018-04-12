
#include <ff/pipeline.hpp>
#include <stages/BaseStage.hpp>
#include <pipeline/Farm.hpp>

using namespace ff;

namespace PpFf {

    class Pipeline {
    public:
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
                pipeline.add_stage(stage->workers[0]);
            } else {
                allocateFarm(); // Lazy allocation.
                for (unsigned int i = 0; i < no_workers; i++) {
                    ff_pipeline *worker_pipe = (ff_pipeline*) farm->getWorker(i);
                    worker_pipe->add_stage(stage->workers[i]);
                }
            }
        }

        int nbWorkers() {
            return no_workers;
        }

        void setNbWorkers(int no_workers) {
            if (this->no_workers != 1) return;

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

            farm = new Farm(no_workers);
            pipeline.add_stage(farm->getFarm());
        }

    private:
        unsigned int no_workers;
        std::vector<IStage*> stages;
        ff_pipeline pipeline;
        Farm* farm;
    };

}

