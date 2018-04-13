
#include <operators/CountOperator.hpp>
#include <operators/SumOperator.hpp>
#include <operators/SourceOperator.hpp>
#include <operators/CollectorOperator.hpp>
#include <operators/FindOperator.hpp>
#include <operators/MapOperator.hpp>
#include <operators/FlatOperator.hpp>
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
#include <pipeline/Pipeline.hpp>
#include <stages/Stage.hpp>
#include <stages/Collectors.hpp>
#include <stages/BaseStage.hpp>
#include <utilities/Debug.hpp>
#include <vector>
#include <map>
#include <unordered_map>

#include <operators/ReduceByKeyOperator2.hpp>

using namespace PpFf;

namespace PpFf {

    class Pipe {
    public:
        Pipe() {};

        ~Pipe() {};

        Pipe& parallel(int no_workers = 1) {
            pipe.setNbWorkers(no_workers);

            return *this;
        };


        template < typename T, typename Iterator >
        Pipe& source(Iterator begin, Iterator end) {
            typedef SourceOperator<T, Iterator> Source;

            Stage<Source>* stage = new Stage<Source>();
            stage->addOperator(pipe.nbWorkers(), begin, end);
            pipe.addStage(stage);

            return *this;
        }

        Pipe& linesFromFile(const std::string& path) {
            typedef LinesFromFileOperator LinesFromFile;

            BaseStage<LinesFromFile>* stage = new BaseStage<LinesFromFile>();
            stage->addOperator(pipe.nbWorkers(), path);
            pipe.addStage(stage);

            return *this;
        }

        unsigned int count() {
            typedef CountOperator<int> Count;

            Collectors<Count>* collectors = new Collectors<Count>();
            collectors->addOperator(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        T sum() {
            typedef SumOperator<T> Sum;

            Collectors<Sum>* collectors = new Collectors<Sum>();
            collectors->addOperator(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T,
                   template < typename ELEM, class ALLOC = std::allocator<ELEM>>
                   class TContainer >
            TContainer<T> collect() {
            typedef CollectorOperator<T, TContainer<T>> Collector;

            Collectors<Collector>* collectors = new Collectors<Collector>();
            collectors->addOperator(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, typename Out >
        Pipe& map(std::function< Out*(In*) > const& taskFunc) {
            typedef MapOperator<In, Out> Map;

            BaseStage<Map>* stage = new BaseStage<Map>();
            stage->addOperator(pipe.nbWorkers(), taskFunc);
            pipe.addStage(stage);

            return *this;
        }

        template < typename In >
        Pipe& find(std::function< bool(In*) > const& taskFunc) {
            typedef FindOperator<In> Find;
            
            BaseStage<Find>* stage = new BaseStage<Find>();
            stage->addOperator(pipe.nbWorkers(), taskFunc);
            pipe.addStage(stage);

            return *this;
        }

        template< typename In, typename Out, typename OutContainer >
        Pipe& flatMap(std::function< OutContainer*(In*) > const& taskFunc) {
            typedef MapOperator<In, OutContainer> Map;
            typedef FlatOperator<OutContainer, Out> Flat;

            BaseStage<Map>* mapStage = new BaseStage<Map>();
            BaseStage<Flat>* flatStage = new BaseStage<Flat>();

            mapStage->addOperator(pipe.nbWorkers(), taskFunc);
            flatStage->addOperator(pipe.nbWorkers());

            pipe.addStage(mapStage);
            pipe.addStage(flatStage);

            return *this;
        };

        template< typename In, typename Out, typename OutContainer = In >
        Pipe& flatMap() {
            typedef FlatOperator<In, Out> Flat;
            
            BaseStage<Flat>* stage = new BaseStage<Flat>();
            stage->addOperator(pipe.nbWorkers());
            pipe.addStage(stage);

            return *this;
        };

        template < typename In >
        Pipe& peek(std::function< void(In*) > const& taskFunc) {
            typedef PeekOperator<In> Peek;
            
            BaseStage<Peek>* stage = new BaseStage<Peek>();
            stage->addOperator(pipe.nbWorkers(), taskFunc);
            pipe.addStage(stage);

            return *this;
        }

        template < typename In, typename Out = In >
        Out reduce(Reducer<In, Out> const& reducer) {
            typedef ReduceOperator<In, Out> Reduce;
            
            Collectors<Reduce>* collectors = new Collectors<Reduce>();
            collectors->addOperator(pipe.nbWorkers(), reducer);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, typename Out = In >
        // Cas special ou accumulator = combiner.
        Out reduce(Out initialValue, std::function<Out (In, Out)> accumulator) {
            typedef ReduceOperator<In, Out> Reduce;
            
            Reducer<In, Out> reducer(initialValue, accumulator, accumulator);
            Collectors<Reduce>* collectors = new Collectors<Reduce>();
            collectors->addOperator(pipe.nbWorkers(), reducer);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, 
                   typename K = In, 
                   typename V = In,
                   typename MapType = std::unordered_map<K, std::vector<V>> >
            MapType groupByKey(std::function< K*(In*)> const& taskFuncOnKey) {
            typedef GroupByKeyOperator<In, K, V, MapType, false> GroupByKey;
            
            Collectors<GroupByKey>* collectors = new Collectors<GroupByKey>();
            collectors->addOperator(pipe.nbWorkers(), taskFuncOnKey);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, std::vector<V>> >
            MapType groupByKey(std::function<K*(In*)> const& taskFuncOnKey, std::function<V*(In*)> const& taskFuncOnValue) {
            typedef GroupByKeyOperator<In, K, V, MapType, true> GroupByKey;
            
            Collectors<GroupByKey>* collectors = new Collectors<GroupByKey>();
            collectors->addOperator(pipe.nbWorkers(), taskFuncOnKey, taskFuncOnValue);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, V> >
        MapType reduceByKey(Reducer<In, V> const& reducer) {
            typedef ReduceByKeyOperator<In, K, V, MapType, false> ReduceByKey;
            
            Collectors<ReduceByKey>* collectors = new Collectors<ReduceByKey>();
            collectors->addOperator(pipe.nbWorkers(), reducer);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, V> >
        MapType reduceByKey(std::function<K*(In*)> const& taskFuncOnKey, Reducer<In, V> const& reducer) {
            typedef ReduceByKeyOperator<In, K, V, MapType, true> ReduceByKey;
            
            Collectors<ReduceByKey>* collectors = new Collectors<ReduceByKey>();
            collectors->addOperator(pipe.nbWorkers(), taskFuncOnKey, reducer);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, V> >
        MapType reduceByKey2(std::function< void(V*, In*) > const& accumulator, std::function< void(V*, V*) > const& combiner) {
            typedef ReduceByKeyOperator2<In, K, V, MapType> ReduceByKey;
            
            Collectors<ReduceByKey>* collectors = new Collectors<ReduceByKey>();
            collectors->addOperator(pipe.nbWorkers(), accumulator, combiner);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        T min(std::function< void(T*, T*) > compare = ([](T* a, T* b) { if (*a> *b) *a = *b;})) {
            typedef MinOperator<T> Min;
            
            Collectors<Min>* collectors = new Collectors<Min>();
            collectors->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        T max(std::function< void(T*, T*) > compare = ([](T* a, T* b) { if (*a <*b) *a = *b;})) {
            typedef MaxOperator<T> Max;
            
            Collectors<Max>* collectors = new Collectors<Max>();
            collectors->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        bool anyMatch(std::function< bool(T*) > predicate) {
            typedef AnyMatchOperator<T> AnyMatch;
            
            Collectors<AnyMatch>* collectors = new Collectors<AnyMatch>();
            collectors->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        bool noneMatch(std::function< bool(T*) > predicate) {
            typedef NoneMatchOperator<T> NoneMatch;
            
            Collectors<NoneMatch>* collectors = new Collectors<NoneMatch>();
            collectors->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }


        template < typename T >
        bool allMatch(std::function< bool(T*) > predicate) {
            typedef AllMatchOperator<T> AllMatch;
            
            Collectors<AllMatch>* collectors = new Collectors<AllMatch>();
            collectors->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }


        template < typename T >
        Pipe& limit(int n) {
            typedef LimitOperator<T> Limit;
            
            BaseStage<Limit>* stage = new BaseStage<Limit>();
            stage->addOperator(pipe.nbWorkers(), n);
            pipe.addStage(stage);

            return *this;
        }

        template < typename T >
        Pipe& skip(int n) {
            typedef SkipOperator<T> Skip;
            
            BaseStage<Skip>* stage = new BaseStage<Skip>();
            stage->addOperator(pipe.nbWorkers(), n);
            pipe.addStage(stage);

            return *this;
        }

        template < typename T,
                   template < typename ELEM, class ALLOC = std::allocator< ELEM > >
                   class TContainer >
        TContainer<T> sort() {
            typedef SortOperator<T, TContainer<T>, false> Sort;
            
            Collectors<Sort>* collectors = new Collectors<Sort>();
            collectors->addOperator(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();
            
            return collectors->value();
        }
        
        template < typename T,
                   template < typename ELEM, class ALLOC = std::allocator< ELEM > >
                   class TContainer >
        TContainer<T> sort(std::function< bool(T, T) > const& compare) {
            typedef SortOperator<T, TContainer<T>, true> Sort;
            
            Collectors<Sort>* collectors = new Collectors<Sort>();
            collectors->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collectors);
            pipe.run();
            
            return collectors->value();
        }
    
    
    private:
        Pipeline pipe;
    };
    
}
