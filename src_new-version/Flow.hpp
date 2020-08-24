
#include <operators/Operator.hpp>
#include <operators/CollectorOperator.hpp>
#include <operators/CountOperator.hpp>
#include <operators/SumOperator.hpp>
#include <operators/FindOperator.hpp>
#include <operators/MapOperator.hpp>
#include <operators/FlatOperator.hpp>
#include <operators/FlatMapOperator.hpp>
#include <operators/PeekOperator.hpp>
#include <operators/ReduceOperator.hpp>
#include <operators/Reducer.hpp>
#include <operators/GroupByKeyOperator.hpp>
#include <operators/ReduceByKeyOperator.hpp>
#include <operators/LinesFromFileOperator.hpp>
#include <operators/MinOperator.hpp>
#include <operators/MaxOperator.hpp>
#include <operators/AllMatchOperator.hpp>
#include <operators/AnyMatchOperator.hpp>
#include <operators/NoneMatchOperator.hpp>
#include <operators/LimitOperator.hpp>
#include <operators/SkipOperator.hpp>
#include <operators/SourceOperator.hpp>
#include <operators/SortOperator.hpp>
#include <collections/Collection.hpp>
#include <stages/FabricOperators.hpp>
#include <utilities/Debug.hpp>
#include <utilities/Identity.hpp>
#include <vector>
#include <map>
#include <unordered_map>

#include <pipeline/Pipeline.hpp>

using namespace PpFf;

namespace PpFf {

    class Flow {
    public:
        Flow() {};

        ~Flow() {};

        static Flow& source(const std::string& path) {
            Flow* flow = new Flow();
            typedef LinesFromFileOperator LinesFromFile;

            FabricOperators<LinesFromFile> fabricOperators;
            flow->pipe.addOperators(fabricOperators.createOperators(1, path));

            return *flow;
        }

        template < typename T, typename Iterator >
        static Flow& source(Iterator begin, Iterator end) {
            Flow* flow = new Flow();
            typedef SourceOperator<T, Iterator> SourceOp;

            FabricOperators<SourceOp> fabricOperators;
            flow->pipe.addOperators(fabricOperators.createOperators(1, begin, end));

            return *flow;
        }

        unsigned int count() {
            typedef CountOperator<int> Count;

            FabricOperators<Count> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers()));    
            Collector<Count> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename T >
        T sum() {
            typedef SumOperator<T> Sum;

            FabricOperators<Sum> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers()));    
            Collector<Sum> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename T,
                   template <typename ELEM, class ALLOC = std::allocator<ELEM>>
                   class TContainer >
            TContainer<T> collect() {
            typedef CollectorOperator<T, TContainer<T>> TOperator;

            FabricOperators<TOperator> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers()));    
            Collector<TOperator> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename In, typename Out >
        Flow& map(std::function<Out*(In*)> const& taskFunc) {
            typedef MapOperator<In, Out> Map;

            FabricOperators<Map> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), taskFunc));

            return *this;
        }

        template < typename In >
        Flow& find(std::function<bool(In*)> const& taskFunc) {
            typedef FindOperator<In> Find;
            
            FabricOperators<Find> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), taskFunc));

            return *this;
        }

        template< typename In, typename TContainer, typename Out >
        Flow& flatMap(std::function<TContainer*(In*)> const& taskFunc) {
            typedef FlatMapOperator<In, TContainer, Out> FlatMap;

            FabricOperators<FlatMap> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), taskFunc));

            return *this;
        };

        template< typename In, typename Out, typename OutContainer = In >
        Flow& flatten() {
            typedef FlatOperator<In, Out> Flat;

            FabricOperators<Flat> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers()));

            return *this;
        };

        template < typename In >
        Flow& peek(std::function<void(In*)> const& taskFunc) {
            typedef PeekOperator<In> Peek;
            
            FabricOperators<Peek> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), taskFunc));

            return *this;
        }

        template < typename In, typename Out = In >
        Out reduce(Reducer<In, Out> const& reducer) {
            typedef ReduceOperator<In, Out> Reduce;

            FabricOperators<Reduce> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), reducer));    
            Collector<Reduce> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename In, typename Out = In >
        // Cas special ou accumulator = combiner.
        Out reduce(Out initialValue, std::function<Out (In, Out)> accumulator) {
            typedef ReduceOperator<In, Out> Reduce;

            Reducer<In, Out> reducer(initialValue, accumulator, accumulator);
            FabricOperators<Reduce> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), reducer));    
            Collector<Reduce> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, std::vector<V>> >
            MapType groupByKey(std::function<K*(In*)> const& keyFunction,
                               std::function<V*(In*)> const& valueFunction = identity<In,V>) {
            typedef GroupByKeyOperator<In, K, V, MapType> GroupByKey;

            FabricOperators<GroupByKey> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), keyFunction, valueFunction));    
            Collector<GroupByKey> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, V> >
        MapType reduceByKey(Reducer<In, V> const& reducer,
                            std::function<K*(In*)> const& keyFunction = identity<In,K>) {
            typedef ReduceByKeyOperator<In, K, V, MapType> ReduceByKey;

            FabricOperators<ReduceByKey> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), keyFunction, reducer));    
            Collector<ReduceByKey> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename T >
        T min(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a > *b) *a = *b; })) {
            typedef MinOperator<T> Min;

            FabricOperators<Min> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), compare));    
            Collector<Min> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename T >
        T max(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a < *b) *a = *b; })) {
            typedef MaxOperator<T> Max;

            FabricOperators<Max> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), compare));    
            Collector<Max> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename T >
        bool anyMatch(std::function<bool(T*)> predicate) {
            typedef AnyMatchOperator<T> AnyMatch;

            FabricOperators<AnyMatch> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), predicate));    
            Collector<AnyMatch> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }

        template < typename T >
        bool noneMatch(std::function<bool(T*)> predicate) {
            typedef NoneMatchOperator<T> NoneMatch;

            FabricOperators<NoneMatch> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), predicate));    
            Collector<NoneMatch> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }


        template < typename T >
        bool allMatch(std::function<bool(T*)> predicate) {
            typedef AllMatchOperator<T> AllMatch;

            FabricOperators<AllMatch> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), predicate));    
            Collector<AllMatch> collector;    
            pipe.addCollector(collector);
            pipe.run();

            return collector.value();
        }


        template < typename T >
        Flow& limit(int n) {
            typedef LimitOperator<T> Limit;
            
            FabricOperators<Limit> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), n));

            return *this;
        }

        template < typename T >
        Flow& skip(int n) {
            typedef SkipOperator<T> Skip;
            
            FabricOperators<Skip> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), n));

            return *this;
        }

        template < typename T >
        Collection<T, std::vector, Flow> sort(std::function<bool(T, T)> const& compare = std::less<T>()) {
            typedef SortOperator<T> Sort;

            FabricOperators<Sort> fabricOperators;
            pipe.addOperators(fabricOperators.createOperators(pipe.nbWorkers(), compare));    
            Collector<Sort> collector;    
            pipe.addCollector(collector);
            pipe.run();

            Collection< T, std::vector, Flow > Collection(collector.value());
            return Collection;
        }
		
        Flow& parallel(int no_workers = 1) {
            pipe.setNbWorkers(no_workers);

            return *this;
        };

    private:
        Pipeline pipe;
    };
    
}
