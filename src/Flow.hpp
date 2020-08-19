
#include <operators/CountOperator.hpp>
#include <operators/SumOperator.hpp>
#include <operators/SourceOperator.hpp>
#include <operators/CollectorOperator.hpp>
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
#include <operators/SortOperator.hpp>
#include <collections/Collection.hpp>
#include <stages/CollectorStage.hpp>
#include <stages/BaseStage.hpp>
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

            BaseStage<LinesFromFile, SOURCE_STAGE>* stage = new BaseStage<LinesFromFile, SOURCE_STAGE>();
            stage->addOperator(1, path);
            flow->pipe.addStage(stage);

            return *flow;
        }

        template < typename T, typename Iterator >
        static Flow& source(Iterator begin, Iterator end) {
            Flow* flow = new Flow();
            typedef SourceOperator<T, Iterator> SourceOp;

            BaseStage<SourceOp, SOURCE_STAGE>* stage = new BaseStage<SourceOp, SOURCE_STAGE>();
            stage->addOperator(1, begin, end);
            flow->pipe.addStage(stage);

            return *flow;
        }

        unsigned int count() {
            typedef CountOperator<int> Count;

            CollectorStage<Count>* collector = new CollectorStage<Count>();
            collector->addOperator(pipe.nbWorkers());
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename T >
        T sum() {
            typedef SumOperator<T> Sum;

            CollectorStage<Sum>* collector = new CollectorStage<Sum>();
            collector->addOperator(pipe.nbWorkers());
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename T,
                   template <typename ELEM, class ALLOC = std::allocator<ELEM>>
                   class TContainer >
            TContainer<T> collect() {
            typedef CollectorOperator<T, TContainer<T>> Collector;

            CollectorStage<Collector>* collector = new CollectorStage<Collector>();
            collector->addOperator(pipe.nbWorkers());
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename In, typename Out >
        Flow& map(std::function<Out*(In*)> const& taskFunc) {
            typedef MapOperator<In, Out> Map;

            BaseStage<Map>* stage = new BaseStage<Map>();
            stage->addOperator(pipe.nbWorkers(), taskFunc);
            pipe.addStage(stage);

            return *this;
        }

        template < typename In >
        Flow& find(std::function<bool(In*)> const& taskFunc) {
            typedef FindOperator<In> Find;
            
            BaseStage<Find>* stage = new BaseStage<Find>();
            stage->addOperator(pipe.nbWorkers(), taskFunc);
            pipe.addStage(stage);

            return *this;
        }

        template< typename In, typename TContainer, typename Out >
        Flow& flatMap(std::function<TContainer*(In*)> const& taskFunc) {
            typedef FlatMapOperator<In, TContainer, Out> FlatMap;

            BaseStage<FlatMap>* flatMapStage = new BaseStage<FlatMap>();

            flatMapStage->addOperator(pipe.nbWorkers(), taskFunc);
            pipe.addStage(flatMapStage);

            return *this;
        };

        template< typename In, typename Out, typename OutContainer = In >
        Flow& flatten() {
            typedef FlatOperator<In, Out> Flat;
            
            BaseStage<Flat>* stage = new BaseStage<Flat>();
            stage->addOperator(pipe.nbWorkers());
            pipe.addStage(stage);

            return *this;
        };

        template < typename In >
        Flow& peek(std::function<void(In*)> const& taskFunc) {
            typedef PeekOperator<In> Peek;
            
            BaseStage<Peek>* stage = new BaseStage<Peek>();
            stage->addOperator(pipe.nbWorkers(), taskFunc);
            pipe.addStage(stage);

            return *this;
        }

        template < typename In, typename Out = In >
        Out reduce(Reducer<In, Out> const& reducer) {
            typedef ReduceOperator<In, Out> Reduce;
            
            CollectorStage<Reduce>* collector = new CollectorStage<Reduce>();
            collector->addOperator(pipe.nbWorkers(), reducer);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename In, typename Out = In >
        // Cas special ou accumulator = combiner.
        Out reduce(Out initialValue, std::function<Out (In, Out)> accumulator) {
            typedef ReduceOperator<In, Out> Reduce;
            
            Reducer<In, Out> reducer(initialValue, accumulator, accumulator);
            CollectorStage<Reduce>* collector = new CollectorStage<Reduce>();
            collector->addOperator(pipe.nbWorkers(), reducer);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, std::vector<V>> >
            MapType groupByKey(std::function<K*(In*)> const& keyFunction,
                               std::function<V*(In*)> const& valueFunction = identity<In,V>) {
            typedef GroupByKeyOperator<In, K, V, MapType> GroupByKey;
            
            CollectorStage<GroupByKey>* collector = new CollectorStage<GroupByKey>();
            collector->addOperator(pipe.nbWorkers(), keyFunction, valueFunction);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, V> >
        MapType reduceByKey(Reducer<In, V> const& reducer,
                            std::function<K*(In*)> const& keyFunction = identity<In,K>) {
            typedef ReduceByKeyOperator<In, K, V, MapType> ReduceByKey;
            
            CollectorStage<ReduceByKey>* collector = new CollectorStage<ReduceByKey>();
            collector->addOperator(pipe.nbWorkers(), keyFunction, reducer);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename T >
        T min(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a > *b) *a = *b; })) {
            typedef MinOperator<T> Min;
            
            CollectorStage<Min>* collector = new CollectorStage<Min>();
            collector->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename T >
        T max(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a < *b) *a = *b; })) {
            typedef MaxOperator<T> Max;
            
            CollectorStage<Max>* collector = new CollectorStage<Max>();
            collector->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename T >
        bool anyMatch(std::function<bool(T*)> predicate) {
            typedef AnyMatchOperator<T> AnyMatch;
            
            CollectorStage<AnyMatch>* collector = new CollectorStage<AnyMatch>();
            collector->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }

        template < typename T >
        bool noneMatch(std::function<bool(T*)> predicate) {
            typedef NoneMatchOperator<T> NoneMatch;
            
            CollectorStage<NoneMatch>* collector = new CollectorStage<NoneMatch>();
            collector->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }


        template < typename T >
        bool allMatch(std::function<bool(T*)> predicate) {
            typedef AllMatchOperator<T> AllMatch;
            
            CollectorStage<AllMatch>* collector = new CollectorStage<AllMatch>();
            collector->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collector);
            pipe.run();

            return collector->value();
        }


        template < typename T >
        Flow& limit(int n) {
            typedef LimitOperator<T> Limit;
            
            BaseStage<Limit>* stage = new BaseStage<Limit>();
            stage->addOperator(pipe.nbWorkers(), n);
            pipe.addStage(stage);

            return *this;
        }

        template < typename T >
        Flow& skip(int n) {
            typedef SkipOperator<T> Skip;
            
            BaseStage<Skip>* stage = new BaseStage<Skip>();
            stage->addOperator(pipe.nbWorkers(), n);
            pipe.addStage(stage);

            return *this;
        }

        template < typename T >
        Collection<T, std::vector, Flow> sort(std::function<bool(T, T)> const& compare = std::less<T>()) {
            typedef SortOperator<T> Sort;

            CollectorStage<Sort>* collector = new CollectorStage<Sort>();
            collector->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collector);
            pipe.run();

            Collection< T, std::vector, Flow > Collection(collector->value());
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
