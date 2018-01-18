
#include <ff/pipeline.hpp>
#include <operators/FlatMap.hpp>
#include "utilities/MapType.hpp"
#include "utilities/Collectors.hpp"
#include "operators/Source.hpp"
#include "operators/SourceFromFile.hpp"
#include "operators/LinesFromFile.hpp"
#include "operators/Map.hpp"
#include "operators/Peek.hpp"
#include "operators/Collector.hpp"
#include "operators/Reduce.hpp"
#include "operators/GroupByKey.hpp"
#include "operators/Close.hpp"
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

		template < template < typename ELEM,
		                    class ALLOC = std::allocator< ELEM > >
		                    class TContainer >
		void sourceFromFile(const std::string& path, const std::string& delimiter){
                    pipe.add_stage(new SourceFromFile< TContainer >(path, delimiter));
		}
            
		void linesFromFile(const std::string& path){
                    pipe.add_stage(new LinesFromFile(path));
		}
            
		template < typename In, typename Out >
		void map(std::function< Out*(In*) > const& taskFunc){
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
		void find(std::function< bool(In*) > const& taskFunc){
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
		void peek(std::function< void(In*) > const& taskFunc){
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

			//collectors.collect(AS_SUM);

			typedef Container< ACCUM, std::vector > CONTAINER;
			CONTAINER &container = collectors.getContainer();

//			for(int i = 0; i < no_workers; i++){
//				ACCUM accum;
//				container.push(accum);
//			}

			if(!isParallel()){
				pipe.add_stage((new Sum< T >(container[0])));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage((new Sum< T >(container[i])));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		template < typename T >
		void reduce(Collectors< T > &collectors, std::function< void(T*, T*) > const& biOp){
			//pipe.add_stage(new Reduce< T >(identity, biOp));

			typedef Container< T, std::vector > CONTAINER;
			CONTAINER &container = collectors.getContainer();

//			for(int i = 0; i < no_workers; i++){
//				T elem{};
//				container.push(elem);
//			}

			if(!isParallel()){
				pipe.add_stage((new Reduce< T >(container[0], biOp)));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage((new Reduce< T >(container[i], biOp)));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< MapType < K, std::vector< V > > > &collectors){
			//pipe.add_stage(new GroupByKey< In, K, V, false, false >(collectors.getContainer()));

			typedef MapType < K, std::vector< V > > MAP;
			typedef Container< MAP, std::vector > CONTAINER;
			CONTAINER &container = collectors.getContainer();

//			for(int i = 0; i < no_workers; i++){
//				MAP elem{};
//				container.push(elem);
//			}

			if(!isParallel()){
				pipe.add_stage(new GroupByKey< In, K, V, false, false >(container[0]));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new GroupByKey< In, K, V, false, false >(container[i]));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< MapType < K, std::vector< V > > > &collectors, std::function< K*(In*) > const& taskFunc){
			//pipe.add_stage(new GroupByKey< In, K, V, true, false >(collectors.getContainer(), taskFunc));

			typedef MapType < K, std::vector< V > > MAP;
			typedef Container< MAP, std::vector > CONTAINER;
			CONTAINER &container = collectors.getContainer();

//			for(int i = 0; i < no_workers; i++){
//				MAP elem{};
//				container.push(elem);
//			}

			if(!isParallel()){
				pipe.add_stage(new GroupByKey< In, K, V, true, false >(container[0], taskFunc));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new GroupByKey< In, K, V, true, false >(container[i], taskFunc));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< MapType < K, V > > &collectors, std::function< K*(In*) > taskFunc, std::function< void(V&, In*) > const& binaryOperator){
			//pipe.add_stage(new GroupByKey< In, K, V, true, true >(collectors.getContainer(), taskFunc, binaryOperator));

			typedef MapType < K , V > MAP;
			typedef Container< MAP, std::vector > CONTAINER;
			CONTAINER &container = collectors.getContainer();

//			for(int i = 0; i < no_workers; i++){
//				MAP elem{};
//				container.push(elem);
//			}

			if(!isParallel()){
				pipe.add_stage(new GroupByKey< In, K, V, true, true >(container[0], taskFunc, binaryOperator));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new GroupByKey< In, K, V, true, true >(container[i], taskFunc, binaryOperator));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		template < typename In, typename K, typename V >
		void groupByKey(Collectors< MapType < K, V > > &collectors, std::function< void(V&, In*) > const& binaryOperator){
			//pipe.add_stage(new GroupByKey< In, K, V, false, true >(collectors.getContainer(), binaryOperator));

			typedef MapType < K, V > MAP;
			typedef Container< MAP, std::vector > CONTAINER;
			CONTAINER &container = collectors.getContainer();

//			for(int i = 0; i < no_workers; i++){
//				MAP elem{};
//				container.push(elem);
//			}

			if(!isParallel()){
				pipe.add_stage(new GroupByKey< In, K, V, false, true >(container[0], binaryOperator));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage(new GroupByKey< In, K, V, false, true >(container[i], binaryOperator));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		template < typename T, typename C >
		void collect(Collectors< T, C > &collectors){

			C &container = collectors.getContainer();

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

			//collectors.collect(AS_COUNT);

			typedef Container< ACCUM, std::vector > CONTAINER;
			CONTAINER &container = collectors.getContainer();

//			for(int i = 0; i < no_workers; i++){
//				ACCUM accum;
//				container.push(accum);
//			}

			if(!isParallel()){
				pipe.add_stage((new Count< ACCUM >(container[0])));
			}else{
				utilities::Farm *farm = InstantiateFarm();

				for(int i = 0; i < no_workers; i++){
					ff_pipeline *worker_pipe = (ff_pipeline*)farm->getWorker(i);
					worker_pipe->add_stage((new Count< ACCUM >(container[i])));
				}

				pipe.add_stage(farm->getFarm());
			}
		}

		void close(){
			pipe.add_stage(new Close());
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


