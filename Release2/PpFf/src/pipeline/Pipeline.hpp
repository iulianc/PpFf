
#include <ff/pipeline.hpp>
#include <stages/BaseStage.hpp>

using namespace ff;

namespace PpFf{

	class Pipeline{
	public:
		Pipeline(): no_workers(1){}
		~Pipeline(){
			for (unsigned int i = 0; i < stages.size(); i++) {
				delete (stages[i]);
			}

			stages.clear();
		}

		template< typename T >
		T* createStage(){
			T *stage = new T();
			stages.push_back(stage);
			return stage;
		}

		template< typename T >
		void addStage(T *stage){
			assert(stage->workers.size() == no_workers);

			if(!isParallel()){
				pipeline.add_stage(&(stage->workers[0]));
			} else {
				//Farm *farm = InstantiateFarm();
				//.......

				for(unsigned int i = 0; i < no_workers; i++){
				   pipeline.add_stage(&(stage->workers[i]));
				}
			}
		}

		int getWorkers(){
			return no_workers;
		}

		void setWorkers(int no_workers){
			if(this->no_workers == 1){
				this->no_workers = no_workers;
			}
		}

		bool isParallel(){
			return no_workers > 1;
		}

		void run(){
			if (pipeline.run_and_wait_end() < 0) error("running pipeline");
		}

	private:
		unsigned int no_workers;
		std::vector< IStage* > stages;
		ff_pipeline pipeline;

	};

}

