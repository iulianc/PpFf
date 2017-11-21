#include <iostream>
#include <vector>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include "operators/Find.hpp"
#include "utilities/Collectors.hpp"
#include "utilities/NullType.hpp"
#include "operators/Count.hpp"
#include <functional>

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

		template < template < typename ELEM,
		                    class ALLOC = std::allocator< ELEM > >
		                    class TContainer >
		Pipe& sourceFromFile(const std::string& path, const std::string& delimiter = " "){
			stageManager->sourceFromFile< TContainer >(path, delimiter);
			return *this;
		}

		template < typename In, typename Out >
		Pipe& map(std::function< Out(In) > const& taskFunc){
			stageManager->map< In, Out >(taskFunc);
			return *this;
		}

	    template< typename In, typename Out, typename ContainerFuncOut >
	    Pipe& flatMap(std::function< ContainerFuncOut(In) > const& taskFunc){
	    	stageManager->map< In, ContainerFuncOut >(taskFunc);
	    	stageManager->flat< ContainerFuncOut, Out >();
	    	return *this;
	    };

	    template< typename InContainer, typename Out, typename ContainerFuncOut = InContainer >
	    Pipe& flatMap(){
	    	stageManager->flat< ContainerFuncOut, Out >();
	    	return *this;
	    };

		template < typename In >
		Pipe& find(std::function< bool(In) > const& taskFunc){
			stageManager->find< In >(taskFunc);
			return *this;
		}

		template < typename In >
		Pipe& peek(std::function< void(In) > const& taskFunc){
			stageManager->peek< In >(taskFunc);
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

//			typedef Accumulator< T > ACCUM;
//			T *identity = new T;
//			ACCUM init;
//			ACCUM result;
//			//result = this->reduce< T, T >(init, [](T sum, T elem)->T {return sum += elem;});
//			result = this->reduce< ACCUM, ACCUM >(init, ([](T sum, T elem)->T {return sum += elem;}));
//			this->run();
//			return result;
		}

		template < typename In, typename Out = In >
		Out reduce(std::function< Out(Out, Out) > const& biOp){
			Out result;
			//result->age = 0;
			stageManager->reduce< Out >(result, biOp);
			this->run();
			return result;
		}

		template < typename In, typename Out = In >
		Out reduce(Out identity, std::function< Out(Out, Out) > const& biOp){
			Out result = identity;
			stageManager->reduce< Out >(result, biOp);
			this->run();
			return result;
		}

		//
		template < typename In, typename Out >
		Out reduce(Out identity, std::function< Out(In) > const& taskFunc, std::function< Out(Out, Out) > const& biOp){
			Out result = identity;
			stageManager->map< In, Out >(taskFunc);
			stageManager->reduce< Out >(result, biOp);
			this->run();
			return result;
		}

		template < typename In, typename K = In, typename V = In >
		std::map < K, std::vector< V > > groupByKey(){
			typedef std::map < K, std::vector< V > > CONTAINER;
			Collectors< K, V, CONTAINER > collectors;
			stageManager->groupByKey< In, K, V >(collectors);
			this->run();
			return collectors.template value();
		}

		template < typename In, typename K = In, typename V = In >
		std::map < K, std::vector< V > > groupByKey(std::function< K(In) > const& taskFunc){
			typedef std::map < K, std::vector< V > > CONTAINER;
			Collectors< K, V, CONTAINER > collectors;
			stageManager->groupByKey< In, K, V >(collectors, taskFunc);
			this->run();
			return collectors.template value();
		}

		template < typename In, typename K = In, typename V = In >
		std::map < K, V > groupByKey(std::function< K(In) > taskFunc, V identity, std::function< V(V, In) > const& binaryOperator){
			typedef std::map < K, V > CONTAINER;
			Collectors< K, V, CONTAINER > collectors;
			stageManager->groupByKey< In, K, V >(collectors, taskFunc, identity, binaryOperator);
			this->run();
			return collectors.template value();
		}

		template < typename In, typename K = In, typename V = In >
		std::map < K, V > groupByKey(V identity, std::function< V(V, In) > const& binaryOperator){
			typedef std::map < K, V > CONTAINER;
			Collectors< K, V, CONTAINER > collectors;
			stageManager->groupByKey< In, K, V >(collectors, identity, binaryOperator);
			this->run();
			return collectors.template value();
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



