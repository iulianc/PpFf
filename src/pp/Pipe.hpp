#include <iostream>
#include <vector>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include "operators/Find.hpp"
#include "utilities/Collectors.hpp"
#include "operators/Count.hpp"

#include "operators/Sum.hpp"
#include "StageManager.hpp"

using namespace ff;

namespace pp{

	//enum class Collector {Undefined, ToList, ToVector};

	class Pipe {
	public:

		Pipe(): no_workers(1){
			stageManager = new StageManager(pipe, no_workers);
		};
		~Pipe(){};

		template < typename T, typename Iterator >
		Pipe& source(Iterator begin, Iterator end){
			stageManager->source< T, Iterator >(begin, end);
			return *this;
		}

		template < typename In, typename Out, typename TaskFunc >
		Pipe& map(TaskFunc taskf){
			stageManager->map< In, Out, TaskFunc >(taskf);
			//this->add_stage(new Map< In, Out, TaskFunc >(taskf));
			return *this;
		}

		template < typename T, typename TaskFunc >
		Pipe& find(TaskFunc const& taskf){
			stageManager->find< T, TaskFunc >(taskf);
			//this->add_stage(new Find< In, TaskFunc >(taskf));
			return *this;
		}

		template < typename T, typename TaskFunc >
		Pipe& peek(TaskFunc const& taskf){
			stageManager->peek< T, TaskFunc >(taskf);
			return *this;
		}

		template <typename T,
		          template <typename ELEM,
		                    class ALLOC = std::allocator<ELEM> >
		                    class CONT >
		CONT<T> collect() {
			typedef Container< T, CONT > CONTAINER;
			Collectors< T, CONTAINER > collectors;
			stageManager->collect< T, CONTAINER >(collectors);
			this->run();
			CONTAINER container = collectors.template value();
			return container.value();
		}

		Pipe& parallel(int no_workers = 1){
			if(this->no_workers == 1){
				this->no_workers = no_workers;
			}

			return *this;
		};

		template < typename T >
		T sum(){
			typedef Accumulator< T > ACCUM;
			Collectors< ACCUM > collectors;
			stageManager->sum< T >(collectors);
			this->run();
			ACCUM accum = collectors.template value< ACCUM >();
			return accum.value();
		}

		unsigned int count(){
			typedef Accumulator< unsigned int > ACCUM;
			Collectors< ACCUM > collectors;
			stageManager->count(collectors);

			//this->add_stage(new Count< Accumulator< unsigned int > >(accum));
			this->run();
			ACCUM accum = collectors.template value< ACCUM >();
			return accum.value();
		}


	private:
		void run(){
			if (pipe.run_and_wait_end() < 0) error("running pipe");
		}

		ff_pipeline pipe;
		ff_pipeline pipep;
		int no_workers;
		StageManager* stageManager;
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



