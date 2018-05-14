#ifndef FARM_HPP
#define FARM_HPP

#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <pipeline/Node.hpp>
#include <pipeline/Worker.hpp>

using namespace ff;

namespace PpFf {

	class Farm: public Node {
	public:
		Farm(unsigned int no_workers): no_workers(no_workers) {
			type = NodeTypeFarm;
			farm = new ff_farm<>();

			for (unsigned int i = 0 ; i < no_workers; ++i) {
				Worker *worker = new Worker();
				workers.push_back(worker);
			}

		}

		NodeTypes Type(){
			return type;
		}

        unsigned int nbWorkers() {
            return no_workers;
        }

		void addStage(std::vector< ff_node* > stageWorkers) {
			assert(workers.size() == stageWorkers.size());

			for(unsigned int i = 0; i < workers.size(); i++){
				workers[i]->addStage(stageWorkers[i]);
			}
		}

		ff_node* getNode() {
			std::vector< ff_node* > innerWorkers;
			for( Worker *worker : workers ) {
				ff_node *node = worker->getNode();
				innerWorkers.push_back(node);
			}

			farm->add_workers(innerWorkers);
			return farm;
		}


	private:
		unsigned int no_workers;
		std::vector< Worker* > workers;
		ff_farm<> *farm;
	};



//    enum {
//        DEF_IN_BUFF_ENTRIES=2048,
//        DEF_IN_OUT_DIFF=128,
//        DEF_OUT_BUFF_ENTRIES=(DEF_IN_BUFF_ENTRIES+DEF_IN_OUT_DIFF)
//    };
//
//    class OldFarm {
//    public:
//        OldFarm(int no_workers): _no_workers(no_workers) {
//            for (unsigned int i = 0 ; i < _no_workers; ++i) {
//                ff_pipeline *pipe = new ff_pipeline();
//                _workers.push_back(pipe);
//            }
//
//            //			std::unique_ptr<ff::ff_node> emitter = std::unique_ptr<ff::ff_node>(nullptr);
//            //			std::unique_ptr<ff::ff_node> collector = std::unique_ptr<ff::ff_node>(nullptr);
//            //_farm = new ff_farm<>(false, DEF_IN_BUFF_ENTRIES, DEF_OUT_BUFF_ENTRIES, false, _workers.size());
//
//            _farm.add_workers(_workers);
//            _farm.add_collector(new Empty());
//        };
//
//        ff_node* getWorker(unsigned int worker_id) {
//            if (worker_id >= _no_workers) throw std::out_of_range("out of range");
//
//            return _workers[worker_id];
//        }
//
//        ff_farm<>* getFarm() {
//            return &_farm;
//        }
//
//
//    private:
//        unsigned int _no_workers;
//        ff_farm<> _farm;
//        std::vector<ff_node*> _workers;
//    };




}

#endif
