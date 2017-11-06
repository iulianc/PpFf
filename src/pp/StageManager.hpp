
#include <ff/pipeline.hpp>
#include "utilities/Collectors.hpp"
#include "operators/Source.hpp"
#include "operators/Map.hpp"
#include "operators/Peek.hpp"
#include "operators/Collector.hpp"
#include "operators/Flat.hpp"
#include "operators/Reduce.hpp"
#include "operators/GroupByKey.hpp"
#include "utilities/Farm.hpp"
#include <functional>

using namespace ff;

//namespace pp{
	class StageManager {

		utilities::Farm* InstantiateFarm(){
			if(_farm == NULL){
				_farm = new utilities::Farm(no_workers);
			}

			return _farm;
		}

	public:
		StageManager(ff_pipeline &pipe, int &no_workers):pipe(pipe), no_workers(no_workers){
			_farm = NULL;
		};

		template < typename T, typename Iterator >
		void source(Iterator begin, Iterator end){
			pipe.add_stage(new Source< T, Iterator >(begin, end));
		}

		template < typename In, typename Out >
		void map(std::function< Out(In) > const& taskFunc){
			if(!isParallel()){
				pipe.add_stage(new Map< In, Out >(taskFunc));
			} else {
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new Map< In, Out >(taskFunc));
				}
			}
		}

		template < typename TContainer, typename Out >
		void flat(){
			if(!isParallel()){
				pipe.add_stage(new Flat< TContainer, Out >);
			} else {
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new Flat< TContainer, Out >);
				}
			}
		}

		template < typename In >
		void find(std::function< bool(In) > const& taskFunc){
			if(!isParallel()){
				pipe.add_stage(new Find< In >(taskFunc));
			} else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new Find< In >(taskFunc));
				}
			}
		}

		template < typename In >
		void peek(std::function< void(In) > const& taskFunc){
			if(!isParallel()){
				pipe.add_stage(new Peek< In >(taskFunc));
			} else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new Peek< In >(taskFunc));
				}
			}
		}

		template < typename T, typename ACCUM = Accumulator< T > >
		void sum(Collectors< ACCUM > &collectors){

			collectors.collect(AS_SUM);

			for(int i = 0; i < no_workers; i++){
				ACCUM accum;
				collectors.getContainer().push(accum);
			}


			if(!isParallel()){
				pipe.add_stage((new Sum< T >(collectors.getContainer().at(0))));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage((new Sum< T >(collectors.getContainer().at(i))));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		template < typename T >
		void reduce(T &identity, std::function< T(T, T) > const& biOp){
			pipe.add_stage(new Reduce< T >(identity, biOp));
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< K, V, std::map < K, std::vector< V > > > &collectors){
			pipe.add_stage(new GroupByKey< In, K, V, false, false >(collectors.getContainer()));
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< K, V, std::map < K , std::vector< V > > > &collectors, std::function< K(In) > const& taskFunc){
			pipe.add_stage(new GroupByKey< In, K, V, true, false >(collectors.getContainer(), taskFunc));
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< K, V, std::map < K , V > > &collectors, std::function< K(In) > taskFunc, V identity, std::function< V(V, In) > const& binaryOperator){
			pipe.add_stage(new GroupByKey< In, K, V, true, true >(collectors.getContainer(), taskFunc, identity, binaryOperator));
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< K, V, std::map < K , V > > &collectors, V identity, std::function< V(V, In) > const& binaryOperator){
			pipe.add_stage(new GroupByKey< In, K, V, false, true >(collectors.getContainer(), identity, binaryOperator));
		}

		template < typename T, typename C >
		void collect(Collectors< T, T, C > &collectors){

			if(!isParallel()){
				pipe.add_stage((new Collector< T, C >(collectors.getContainer())));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				pipe.add_stage(farm->getFarm());
				pipe.add_stage((new Collector< T, C >(collectors.getContainer())));
			}
		}

		template< typename ACCUM = Accumulator< unsigned int > >
		void count(Collectors< ACCUM > &collectors){

			collectors.collect(AS_COUNT);

			for(int i = 0; i < no_workers; i++){
				ACCUM accum;
				collectors.getContainer().push(accum);
			}


			if(!isParallel()){
				pipe.add_stage((new Count< ACCUM >(collectors.getContainer().at(0))));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage((new Count< ACCUM >(collectors.getContainer().at(i))));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		bool isParallel(){
			return no_workers > 1;
		}

	private:
		ff_pipeline &pipe;
		int &no_workers;
		utilities::Farm *_farm;
	};
//}


