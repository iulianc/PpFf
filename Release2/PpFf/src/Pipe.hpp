
#include <operators/CountOperator.hpp>
#include <operators/SumOperator.hpp>
#include <operators/SourceOperator.hpp>
#include <operators/CollectorOperator.hpp>
#include <operators/FindOperator.hpp>
#include <operators/MapOperator.hpp>
#include <operators/FlatOperator.hpp>
#include <operators/PeekOperator.hpp>
#include <operators/GroupByKeyOperator.hpp>
#include <pipeline/Pipeline.hpp>
#include <stages/Stage.hpp>
#include <stages/Collectors.hpp>
#include <stages/BaseStage.hpp>
#include <vector>
#include <map>
#include <unordered_map>

using namespace PpFf;

namespace PpFf{

	class Pipe{
	public:
		~Pipe(){};

		template < typename T, typename Iterator >
		Pipe& source(Iterator begin, Iterator end){
			typedef SourceOperator< T, Iterator > Source;
			typedef Stage< Source > StageCount;

			StageCount *stage = pipe.createStage< StageCount >();
			stage->createOperators(pipe.getWorkers(), begin, end);
			pipe.addStage(stage);
			return *this;
		}

		unsigned int count(){
            typedef CountOperator< int > Count;
            typedef Collectors< Count > StageCollectors;

            StageCollectors *collectors = pipe.createStage< StageCollectors >();
            collectors->createOperators(pipe.getWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
		}

		template < typename T >
		T sum(){
            typedef SumOperator< T > Sum;
            typedef Collectors< Sum > StageCollectors;

            StageCollectors *collectors = pipe.createStage< StageCollectors >();
            collectors->createOperators(pipe.getWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
		}

		Pipe& parallel(int no_workers = 1){
			pipe.setWorkers(no_workers);
			return *this;
		};


		template <typename T,
		          template < typename ELEM,
		                    class ALLOC = std::allocator< ELEM > >
		                    class TContainer >
		TContainer< T > collect() {
            typedef CollectorOperator< T, TContainer< T > > Collector;
            typedef Collectors< Collector > StageCollectors;

            StageCollectors *collectors = pipe.createStage< StageCollectors >();
            collectors->createOperators(pipe.getWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
		}

		template < typename In, typename Out >
		Pipe& map(std::function< Out*(In*) > const& taskFunc){
			typedef MapOperator< In, Out > Map;
			typedef BaseStage< Map > Stage;

			Stage *stage = pipe.createStage< Stage >();
			stage->createOperators(pipe.getWorkers(), taskFunc);
			pipe.addStage(stage);

			return *this;
		}

		template < typename In >
		Pipe& find(std::function< bool(In*) > const& taskFunc){
			typedef FindOperator< In > Find;
			typedef BaseStage< Find > Stage;

			Stage *stage = pipe.createStage< Stage >();
			stage->createOperators(pipe.getWorkers(), taskFunc);
			pipe.addStage(stage);

			return *this;
		}

	    template< typename In, typename Out, typename OutContainer >
	    Pipe& flatMap(std::function< OutContainer*(In*) > const& taskFunc){
			typedef MapOperator< In, OutContainer > Map;
			typedef FlatOperator< OutContainer, Out > Flat;
			typedef BaseStage< Map > MapStage;
			typedef BaseStage< Flat > FlatStage;

			MapStage *mapStage = pipe.createStage< MapStage >();
			FlatStage *flatStage = pipe.createStage< FlatStage >();

			mapStage->createOperators(pipe.getWorkers(), taskFunc);
			flatStage->createOperators(pipe.getWorkers());

			pipe.addStage(mapStage);
			pipe.addStage(flatStage);

			return *this;
	    };

	    template< typename In, typename Out, typename OutContainer = In >
	    Pipe& flatMap(){
			typedef FlatOperator< In, Out > Flat;
			typedef BaseStage< Flat > Stage;

			Stage *stage = pipe.createStage< Stage >();
			stage->createOperators(pipe.getWorkers());
			pipe.addStage(stage);

			return *this;
	    };

		template < typename In >
		Pipe& peek(std::function< void(In*) > const& taskFunc){
			typedef PeekOperator< In > Peek;
			typedef BaseStage< Peek > Stage;

			Stage *stage = pipe.createStage< Stage >();
			stage->createOperators(pipe.getWorkers(), taskFunc);
			pipe.addStage(stage);

			return *this;
		}

		template < typename In, typename K = In, typename V = In,
				   typename MapType = std::unordered_map< K, std::vector< V > > >
		MapType groupByKey(std::function< K*(In*) > const& taskFuncOnKey){
            typedef GroupByKeyOperator< In, K, V, MapType, false > GroupByKey;
            typedef Collectors< GroupByKey > StageCollectors;

            StageCollectors *collectors = pipe.createStage< StageCollectors >();
            collectors->createOperators(pipe.getWorkers(), taskFuncOnKey);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
		}

		template < typename In, typename K = In, typename V = In,
				   typename MapType = std::unordered_map< K, std::vector< V > > >
		MapType groupByKey(std::function< K*(In*) > const& taskFuncOnKey, std::function< V*(In*) > const& taskFuncOnValue){
            typedef GroupByKeyOperator< In, K, V, MapType, true > GroupByKey;
            typedef Collectors< GroupByKey > StageCollectors;

            StageCollectors *collectors = pipe.createStage< StageCollectors >();
            collectors->createOperators(pipe.getWorkers(), taskFuncOnKey, taskFuncOnValue);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
		}

	private:
		Pipeline pipe;
	};

}
