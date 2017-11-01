#include <iostream>
#include <vector>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include "operators/Find.hpp"
#include "utilities/Collectors.hpp"
#include "utilities/NullType.hpp"
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
			return *this;
		}

	    template< typename In, typename Out, typename ContainerFuncOut, typename TaskFunc >
	    Pipe& flatMap(TaskFunc taskFunc){
	    	stageManager->map< In, ContainerFuncOut, TaskFunc >(taskFunc);
	    	stageManager->flat< ContainerFuncOut, Out >();
	    	return *this;
	    };

	    template< typename InContainer, typename Out, typename ContainerFuncOut = InContainer, typename TaskFunc = NULL_TYPE >
	    Pipe& flatMap(){
	    	stageManager->flat< ContainerFuncOut, Out >();
	    	return *this;
	    };

		template < typename T, typename TaskFunc >
		Pipe& find(TaskFunc const& taskf){
			stageManager->find< T, TaskFunc >(taskf);
			return *this;
		}

		template < typename T, typename TaskFunc >
		Pipe& peek(TaskFunc const& taskf){
			stageManager->peek< T, TaskFunc >(taskf);
			return *this;
		}

		template <typename T,
		          template < typename ELEM,
		                    class ALLOC = std::allocator< ELEM > >
		                    class TContainer >
		TContainer< T > collect() {
			typedef Container< T, TContainer > CONTAINER;
			Collectors< T, T, CONTAINER > collectors;
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

		template < typename In, typename Out = In, typename BinaryOperator, typename TaskFunc = NULL_TYPE >
		Out reduce(Out identity, BinaryOperator biOp){
			stageManager->reduce< Out, BinaryOperator >(identity, biOp);
			this->run();
			return identity;
		}

		template < typename In, typename Out = In, typename BinaryOperator, typename TaskFunc = NULL_TYPE >
		Out reduce(BinaryOperator biOp){
			Out out;
			stageManager->reduce< Out, BinaryOperator >(out, biOp);
			this->run();
			return out;
		}

		template < typename In, typename Out, typename BinaryOperator, typename TaskFunc >
		Out reduce(Out identity, BinaryOperator biOp, TaskFunc taskf){
			stageManager->map< In, Out, TaskFunc >(taskf);
			stageManager->reduce< Out, BinaryOperator >(identity, biOp);
			this->run();
			return identity;
		}

		template < typename In, typename K = In, typename V = In, typename TaskFunc = NULL_TYPE, typename BinaryOperator = NULL_TYPE >
		std::map < K, std::vector< V > > groupByKey(){
			typedef std::map < K, std::vector< V > > CONTAINER;
			Collectors< K, V, CONTAINER > collectors;
			stageManager->groupByKey< In, K, V, NULL_TYPE, NULL_TYPE >(collectors);
			this->run();
			return collectors.template value();
		}

		template < typename In, typename K = In, typename V = In, typename TaskFunc, typename BinaryOperator = NULL_TYPE >
		std::map < K, std::vector< V > > groupByKey(TaskFunc taskFunc){
			typedef std::map < K, std::vector< V > > CONTAINER;
			Collectors< K, V, CONTAINER > collectors;
			stageManager->groupByKey< In, K, V, TaskFunc, BinaryOperator >(collectors, taskFunc);
			this->run();
			return collectors.template value();
		}

		template < typename In, typename K = In, typename V = In, typename TaskFunc, typename BinaryOperator >
		std::map < K, V > groupByKey(TaskFunc taskFunc, V identity, BinaryOperator binaryOperator){
			typedef std::map < K, V > CONTAINER;
			Collectors< K, V, CONTAINER > collectors;
			stageManager->groupByKey< In, K, V, TaskFunc, BinaryOperator >(collectors, taskFunc, identity, binaryOperator);
			this->run();
			return collectors.template value();
		}

		template < typename In, typename K = In, typename TaskFunc = NULL_TYPE, typename BinaryOperator = NULL_TYPE >
		void groupBy(){
			this->run();
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
		//ff_pipeline pipep;
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



