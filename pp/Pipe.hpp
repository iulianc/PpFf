#include <iostream>
#include <vector>
#include "ff/pipeline.hpp"
#include "ff/farm.hpp"
#include "operators/Map.hpp"
#include "operators/Find.hpp"
#include "operators/AddToPipe.hpp"
#include "operators/Collector.hpp"
#include "operators/Count.hpp"
#include "operators/Sum.hpp"

using namespace ff;

namespace pp{

	enum class Collector {Undefined, ToList, ToVector};

	class Pipe {
	public:

		Pipe(): is_parallel(false), nworkers(2){};
		~Pipe(){};

		template < typename T, typename Iterator >
		Pipe& add_to_pipe(Iterator begin, Iterator end){
			this->add_stage(new AddToPipe< T, Iterator >(begin, end));
			return *this;
		}

		template < typename In, typename Out, typename TaskFunc >
		Pipe& map(TaskFunc const& taskf){
			this->add_stage(new Map< In, Out, TaskFunc >(taskf));
			return *this;
		}

		template < typename In, typename TaskFunc >
		Pipe& find(TaskFunc const& taskf){
			this->add_stage(new Find< In, TaskFunc >(taskf));
			return *this;
		}

		template < typename T >
		void collect(Collector collector);

		Pipe& parallel(){
			is_parallel = true;
			return *this;
		};

		template < typename T >
		T sum(){
			Accumulator< T > accum;
			this->add_stage(new Sum< T, Accumulator< T > >(accum));
			this->run();
			return accum.value();
		}

		unsigned int count(){
			Accumulator< unsigned int > accum;
			this->add_stage(new Count< Accumulator< unsigned int > >(accum));
			this->run();
			std::cout<<"Total in Pipe: "<<accum.value()<<"\n";
			return accum.value();
		}


	private:

		void add_stage(ff::ff_node *stage){
			if(is_parallel){
				pipep.add_stage(stage);
			}else{
				pipe.add_stage(stage);
			}
		}

		void run(){
			if(is_parallel){
				std::vector< ff_node* > Workers;
				for(int i = 0; i < nworkers; i++){
					Workers.push_back(&pipep);
				}

				std::unique_ptr< ff_node > C  =std::unique_ptr< ff_node >(nullptr);
				ff_farm<> farm;

				farm.add_workers(Workers);
				farm.add_collector(C.get());

				pipe.add_stage(&farm);
				if (pipe.run_and_wait_end()<0) error("running pipe");


			}else{
				if (pipe.run_and_wait_end()<0) error("running pipe");
			}
		}

		ff_pipeline pipe;
		ff_pipeline pipep;
		bool is_parallel;
		int nworkers;
	};


//	void Pipe::add_stage(ff::ff_node *stage){
//		if(is_parallel){
//			pipep.add_stage(stage);
//		}else{
//			pipe.add_stage(stage);
//		}
//	}

//	template < typename T, typename Iterator >
//	Pipe& Pipe::add_to_pipe(Iterator begin, Iterator end){
//		this->add_stage(new AddToPipe< T, Iterator >(begin, end));
//		return *this;
//	}




//	template < typename In, typename Out, typename TaskFunc >
//	Pipe& Pipe::map(TaskFunc const& taskf){
//		//pipe.add_stage(new Map< In, Out, TaskFunc >(TaskFunc));
//		//pipe.add_stage(new Map<int, int, mapF>(FuncMap));
//		//pipe.add_stage(new Find<int, findF>([&](int in){if(in % 2 == 0) return true; return false;}));
//		//pipe.add_stage(new Find<int, findF>(FuncFind));
//		pipe.add_stage(new thirdStage());
//		return *this;
//	}


//	template < typename T >
//	T Pipe::sum(){
//		//T out;
//		Accumulator< T, T > accum;
//		this->add_stage(new Sum< T, Accumulator< T, T > >(accum));
//		this->run();
//		std::cout<<"Total in Pipe: "<<accum.value()<<"\n";
//		return accum.value();
//	}



}



