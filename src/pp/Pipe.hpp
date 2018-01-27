#include <iostream>
#include <vector>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <utilities/Workers.hpp>
#include "utilities/MapType.hpp"
#include "utilities/Enumerators.hpp"
#include "operators/Find.hpp"
#include "utilities/Collectors.hpp"
#include "utilities/MapCollectors.hpp"
#include "utilities/NullType.hpp"
#include "operators/Count.hpp"
#include <functional>
#include "aggregates/Aggregate.hpp"
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

		Pipe& linesFromFile(const std::string& path){
			stageManager->linesFromFile(path);
			return *this;
		}

		template < typename In, typename Out >
		Pipe& map(std::function< Out*(In*) > const& taskFunc){
			stageManager->map< In, Out >(taskFunc);
			return *this;
		}

	    template< typename In, typename Out, typename ContainerFuncOut >
	    Pipe& flatMap(std::function< ContainerFuncOut*(In*) > const& taskFunc){
	    	stageManager->map< In, ContainerFuncOut >(taskFunc);
	    	stageManager->flat< ContainerFuncOut, Out >();
	    	return *this;
	    };

	    template< typename InContainer, typename Out, typename ContainerFuncOut = InContainer >
	    Pipe& flatMap(){
	    	stageManager->flat< ContainerFuncOut, Out >();
	    	return *this;
	    };

	    template< typename In, typename Out, typename ContainerFuncOut >
	    Pipe& flatMap2end(std::function< ContainerFuncOut*(In*) > const& taskFunc){
	    	stageManager->flatMap2end< In, Out, ContainerFuncOut >(taskFunc);
	    	return *this;
	    };

	    template< typename In, typename Out, typename ContainerFuncOut = In >
	    Pipe& flatMap2end(){
	    	stageManager->flatMap2end< In, Out, ContainerFuncOut >();
	    	return *this;
	    };

		template < typename In >
		Pipe& find(std::function< bool(In*) > const& taskFunc){
			stageManager->find< In >(taskFunc);
			return *this;
		}

		template < typename In >
		Pipe& peek(std::function< void(In*) > const& taskFunc){
			stageManager->peek< In >(taskFunc);
			return *this;
		}

		template <typename T,
		          template < typename ELEM,
		                    class ALLOC = std::allocator< ELEM > >
		                    class TContainer >
		TContainer< T > collect() {
			typedef Container< T, TContainer > CONTAINER;
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
			Collectors< ACCUM > collectors(no_workers);
			stageManager->sum< T >(collectors);
			this->run();
			if(isParallel())
				collectors.reduce([](ACCUM *out, ACCUM *in){*out = *out + *in;});
			ACCUM accum = collectors.template value< ACCUM >();
			return accum.value();
		}

		template < typename In, typename Out = In >
		Out reduce(std::function< void(Out*, Out*) > const& biOp){
			Collectors< Out > collectors(no_workers);
			stageManager->reduce< Out >(collectors, biOp);
			this->run();
			if(isParallel())
				collectors.reduce(biOp);
			return collectors.template value< Out >();

//			//TODO delete pointer
//			Out *result = new Out();
//			//result->age = 0;
//			stageManager->reduce< Out >(result, biOp);
//			this->run();
//			return *result;
		}

		template < typename In, typename Out = In >
		Out reduce(Out identity, std::function< void(Out*, Out*) > const& biOp){
			Collectors< Out > collectors(no_workers);
			stageManager->reduce< Out >(collectors, biOp);
			this->run();
			if(isParallel())
				collectors.reduce(biOp);
			biOp(&identity, &collectors.template value< Out >());
			return identity;

//			//TODO delete pointer
//			Out *result = new Out();
//			*result = identity;
//			stageManager->reduce< Out >(result, biOp);
//			this->run();
//			return *result;
		}

		template < typename In, typename Out >
		Out reduce(Out identity, std::function< Out*(In*) > const& taskFunc, std::function< void(Out*, Out*) > const& biOp){
			Collectors< Out > collectors(no_workers);
			stageManager->map< In, Out >(taskFunc);
			stageManager->reduce< Out >(collectors, biOp);
			this->run();
			if(isParallel())
				collectors.reduce(biOp);
			biOp(&identity, &collectors.template value< Out >());
			return identity;

//			//TODO delete pointer
//			Out *result = new Out();
//			*result = identity;
//			stageManager->map< In, Out >(taskFunc);
//			stageManager->reduce< Out >(result, biOp);
//			this->run();
//			return *result;
		}


//
//		//*****GroupByKey-Deprecated**********************************
//		template < typename In, typename K = In, typename V = In >
//		MapType < K, std::vector< V > > groupByKey(){
//			typedef MapType < K, std::vector< V > > CONTAINER;
//			Collectors< CONTAINER > collectors(no_workers);
//			stageManager->groupByKey< In, K, V >(collectors);
//			this->run();
//			if(isParallel())
//				collectors.reduce([](CONTAINER *out, CONTAINER *in)
//					{
//						for (auto it = in->begin(); it != in->end(); it++) {
//							(*out)[it->first].insert((*out)[it->first].end(), (it->second).begin(), (it->second).end());
//						}
//					});
//			return collectors.template value< CONTAINER >();
//		}
//
//		template < typename In, typename K = In, typename V = In >
//		MapType < K, std::vector< V > > groupByKey(std::function< K*(In*) > const& taskFunc){
//			typedef MapType < K, std::vector< V > > CONTAINER;
//			Collectors< CONTAINER > collectors(no_workers);
//			stageManager->groupByKey< In, K, V >(collectors, taskFunc);
//			this->run();
//			if(isParallel())
//				collectors.reduce([](CONTAINER *out, CONTAINER *in)
//					{
//						for (auto it = in->begin(); it != in->end(); it++) {
//							(*out)[it->first].insert((*out)[it->first].end(), (it->second).begin(), (it->second).end());
//						}
//					});
//			return collectors.template value< CONTAINER >();
//		}
//
//		template < typename In, typename K = In, typename V = In >
//		MapType < K, V > groupByKey(std::function< K*(In*) > taskFunc, std::function< void(V&, In*) > const& binaryOperator){
//			typedef MapType < K, V > CONTAINER;
//			Collectors< CONTAINER > collectors(no_workers);
//			stageManager->groupByKey< In, K, V >(collectors, taskFunc, binaryOperator);
//			this->run();
//			if(isParallel())
//				collectors.reduce([](CONTAINER *out, CONTAINER *in)
//					{
//						for (auto it = in->begin(); it != in->end(); it++) {
//							(*out)[it->first] += it->second;
//						}
//					});
//			return collectors.template value< CONTAINER >();
//		}
//
//		template < typename In, typename K = In, typename V = In >
//		MapType < K, V > groupByKey(std::function< void(V&, In*) > const& binaryOperator){
//			typedef MapType < K, V > CONTAINER;
//			Collectors< CONTAINER > collectors(no_workers);
//			stageManager->groupByKey< In, K, V >(collectors, binaryOperator);
//			this->run();
//			if(isParallel())
//				collectors.reduce([](CONTAINER *out, CONTAINER *in)
//					{
//						for (auto it = in->begin(); it != in->end(); it++) {
//							(*out)[it->first] += it->second;
//						}
//					});
//			return collectors.template value< CONTAINER >();
//		}
//		//**************************************************************


		//*****GroupByKey-New Version**********************************
		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
		typename std::enable_if< (AggrOp < 4) , MapType < K, V > >::type
		groupByKey(){
			typedef MapType < K, V > CONTAINER;
			typedef MapCollectors< K, V, CONTAINER > COLLECTORS;

			typedef Workers < COLLECTORS > Workers;
			typedef Aggregate< AggrOp, In, V > Aggr;

			Workers workers(no_workers);
			Aggr aggregate;

			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers, aggregate);
			this->run();
			if(isParallel()){
				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
					{
						CONTAINER &containerIn = in->container();
						CONTAINER &containerOut = out->container();

						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
							V &out = containerOut[it->first];
							V in = it->second;

							aggregate.reduce(&out, &in);
						}
					});
			}

			COLLECTORS *collectors = workers.value();
			return collectors->container();
		}

		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
		typename std::enable_if< (AggrOp < 4) , MapType < K, V > >::type
		groupByKey(std::function< K*(In*) > const& taskFuncOnKey){
			typedef MapType < K, V > CONTAINER;
			typedef MapCollectors< K, V, CONTAINER > COLLECTORS;

			typedef Workers < COLLECTORS > Workers;
			typedef Aggregate< AggrOp, In, V > Aggr;

			Workers workers(no_workers);
			Aggr aggregate;

			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers, taskFuncOnKey, aggregate);
			this->run();
			if(isParallel()){
				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
					{
						CONTAINER &containerIn = in->container();
						CONTAINER &containerOut = out->container();

						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
							V &out = containerOut[it->first];
							V in = it->second;

							aggregate.reduce(&out, &in);
						}
					});
			}

			COLLECTORS *collectors = workers.value();
			return collectors->container();
		}

		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
		typename std::enable_if< (AggrOp < 4) , MapType < K, V > >::type
		groupByKey(std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue){
			typedef MapType < K, V > CONTAINER;
			typedef MapCollectors< K, V, CONTAINER > COLLECTORS;

			typedef Workers < COLLECTORS > Workers;
			typedef Aggregate< AggrOp, In, V > Aggr;

			Workers workers(no_workers);
			Aggr aggregate;

			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers, taskFuncOnKey, taskFuncOnValue, aggregate);
			this->run();
			if(isParallel()){
				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
					{
						CONTAINER &containerIn = in->container();
						CONTAINER &containerOut = out->container();

						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
							V &out = containerOut[it->first];
							V in = it->second;

							aggregate.reduce(&out, &in);
						}
					});
			}

			COLLECTORS *collectors = workers.value();
			return collectors->container();
		}

		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
		typename std::enable_if< (AggrOp > 4) , MapType < K, std::vector< V > > >::type
		groupByKey(){
			typedef MapType < K, std::vector< V > > CONTAINER;
			typedef MapCollectors< K, V, CONTAINER > COLLECTORS;

			typedef Workers < COLLECTORS > Workers;
			typedef Aggregate< AggrOp, In, V > Aggr;

			Workers workers(no_workers);

			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers);
			this->run();
			if(isParallel()){
				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
					{
						CONTAINER &containerIn = in->container();
						CONTAINER &containerOut = out->container();

						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
							std::vector< V > &out = containerOut[it->first];
							out.insert(out.end(), (it->second).begin(), (it->second).end());
						}
					});
			}

			COLLECTORS *collectors = workers.value();
			return collectors->container();
		}

		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
		typename std::enable_if< (AggrOp > 4) , MapType < K, std::vector< V > > >::type
		groupByKey(std::function< K*(In*) > const& taskFuncOnKey){
			typedef MapType < K, std::vector< V > > CONTAINER;
			typedef MapCollectors< K, V, CONTAINER > COLLECTORS;

			typedef Workers < COLLECTORS > Workers;
			typedef Aggregate< AggrOp, In, V > Aggr;

			Workers workers(no_workers);

			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers, taskFuncOnKey);
			this->run();
			if(isParallel()){
				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
					{
						CONTAINER &containerIn = in->container();
						CONTAINER &containerOut = out->container();

						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
							std::vector< V > &out = containerOut[it->first];
							out.insert(out.end(), (it->second).begin(), (it->second).end());
						}
					});
			}

			COLLECTORS *collectors = workers.value();
			return collectors->container();
		}

		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
		typename std::enable_if< (AggrOp > 4) , MapType < K, std::vector< V > > >::type
		groupByKey(std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue){
			typedef MapType < K, std::vector< V > > CONTAINER;
			typedef MapCollectors< K, V, CONTAINER > COLLECTORS;

			typedef Workers < COLLECTORS > Workers;
			typedef Aggregate< AggrOp, In, V > Aggr;

			Workers workers(no_workers);

			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers, taskFuncOnKey, taskFuncOnValue);
			this->run();
			if(isParallel()){
				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
					{
						CONTAINER &containerIn = in->container();
						CONTAINER &containerOut = out->container();

						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
							std::vector< V > &out = containerOut[it->first];
							out.insert(out.end(), (it->second).begin(), (it->second).end());
						}
					});
			}

			COLLECTORS *collectors = workers.value();
			return collectors->container();
		}


//		//Average
//		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
//		typename std::enable_if< (AggrOp == Aggregates::OperatorAvg) , MapType < K, V > >::type
//		groupByKey(){
//			typedef std::pair < V, int > Out;
//
//			typedef MapType < K, Out > CONTAINER;
//			typedef MapCollectors< K, Out, CONTAINER > COLLECTORS;
//
//			typedef Workers < COLLECTORS > Workers;
//			typedef Aggregate< AggrOp, In, Out > Aggr;
//
//			Workers workers(no_workers);
//			Aggr aggregate;
//
//
//			stageManager->groupByKey< In, K, Out, COLLECTORS, Workers, Aggr >(workers, aggregate);
//			this->run();
//			if(isParallel()){
//				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
//					{
//						CONTAINER &containerIn = in->container();
//						CONTAINER &containerOut = out->container();
//
//						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
//							Out &out = containerOut[it->first];
//							Out in = it->second;
//
//							aggregate.reduce(&out, &in);
//						}
//					});
//			}
//
//			COLLECTORS *collectors = workers.value();
//			CONTAINER containerPairValue = collectors->container();
//			Out pairValue;
//			MapType < K, V > result;
//
//			for (auto it = containerPairValue.begin(); it != containerPairValue.end(); it++) {
//				pairValue = it->second;
//				result[it->first] = pairValue.first / pairValue.second;
//			}
//
//			return result;
//		}
//
//		//Average
//		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
//		typename std::enable_if< (AggrOp == Aggregates::OperatorAvg) , MapType < K, V > >::type
//		groupByKey(std::function< K*(In*) > const& taskFuncOnKey){
//			typedef std::pair < V, int > Value;
//
//			typedef MapType < K, Value > CONTAINER;
//			typedef MapCollectors< K, Value, CONTAINER > COLLECTORS;
//
//			typedef Workers < COLLECTORS > Workers;
//			typedef Aggregate< AggrOp, V, Value > Aggr;
//
//			Workers workers(no_workers);
//			Aggr aggregate;
//
//			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers, taskFuncOnKey, aggregate);
//			this->run();
//			if(isParallel()){
//				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
//					{
//						CONTAINER &containerIn = in->container();
//						CONTAINER &containerOut = out->container();
//
//						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
//							Value &out = containerOut[it->first];
//							Value in = it->second;
//
//							aggregate.reduce(&out, &in);
//						}
//					});
//			}
//
//			COLLECTORS *collectors = workers.value();
//			CONTAINER containerPairValue = collectors->container();
//			Value pairValue;
//			MapType < K, V > result;
//
//			for (auto it = containerPairValue.begin(); it != containerPairValue.end(); it++) {
//				pairValue = it->second;
//				result[it->first] = pairValue.first / pairValue.second;
//			}
//
//			return result;
//		}

		//Average
		template < typename In, typename K = In, typename V = In, int AggrOp = Aggregates::Undefined >
		typename std::enable_if< (AggrOp == Aggregates::OperatorAvg) , MapType < K, V > >::type
		groupByKey(std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue){
			typedef std::pair < V, int > Value;

			typedef MapType < K, Value > CONTAINER;
			typedef MapCollectors< K, Value, CONTAINER > COLLECTORS;

			typedef Workers < COLLECTORS > Workers;
			typedef Aggregate< AggrOp, V, Value > Aggr;

			Workers workers(no_workers);
			Aggr aggregate;

			stageManager->groupByKey< In, K, V, COLLECTORS, Workers, Aggr >(workers, taskFuncOnKey, taskFuncOnValue, aggregate);
			this->run();
			if(isParallel()){
				workers.reduce([&](COLLECTORS *out, COLLECTORS *in)
					{
						CONTAINER &containerIn = in->container();
						CONTAINER &containerOut = out->container();

						for (auto it = containerIn.begin(); it != containerIn.end(); it++) {
							Value &out = containerOut[it->first];
							Value in = it->second;

							aggregate.reduce(&out, &in);
						}
					});
			}

			COLLECTORS *collectors = workers.value();
			CONTAINER containerPairValue = collectors->container();
			Value pairValue;
			MapType < K, V > result;

			for (auto it = containerPairValue.begin(); it != containerPairValue.end(); it++) {
				pairValue = it->second;
				result[it->first] = pairValue.first / pairValue.second;
			}

			return result;
		}
		//*********************************************************


		unsigned int count(){
			typedef Accumulator< unsigned int > ACCUM;
			Collectors< ACCUM > collectors(no_workers);
			stageManager->count(collectors);

			//this->add_stage(new Count< Accumulator< unsigned int > >(accum));
			this->run();
			if(isParallel())
				collectors.reduce([](ACCUM *out, ACCUM *in){*out = *out + *in;});
			ACCUM accum = collectors.template value< ACCUM >();
			return accum.value();
		}

		void close(){
			stageManager->close();
			this->run();
		}

		const bool isParallel(){
			return no_workers > 1;
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



