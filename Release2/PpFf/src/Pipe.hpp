
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
#include <pipeline/Pipeline.hpp>
#include <stages/Stage.hpp>
#include <stages/Collectors.hpp>
#include <stages/BaseStage.hpp>
#include <vector>
#include <map>
#include <unordered_map>

#include <operators/ReduceByKeyOperator2.hpp>

using namespace PpFf;

namespace PpFf {

    class Pipe {
    public:
        Pipe() {};

        ~Pipe(){};

        template < typename T, typename Iterator >
        Pipe& source(Iterator begin, Iterator end) {
            typedef SourceOperator<T, Iterator> Source;
            typedef Stage<Source> StageCount;

            StageCount* stage = pipe.createStage<StageCount>();
            stage->createOperators(pipe.nbWorkers(), begin, end);
            pipe.addStage(stage);

            return *this;
        }

        Pipe& linesFromFile(const std::string& path) {
            typedef LinesFromFileOperator LinesFromFile;
            typedef BaseStage<LinesFromFile> Stage;

            Stage* stage = pipe.createStage<Stage>();
            stage->createOperators(pipe.nbWorkers(), path);
            pipe.addStage(stage);

            return *this;
        }

        unsigned int count() {
            typedef CountOperator<int> Count;
            typedef Collectors<Count> StageCollectors;

            StageCollectors* collectors = pipe.createStage<StageCollectors>();
            collectors->createOperators(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        T sum() {
            typedef SumOperator<T> Sum;
            typedef Collectors<Sum> StageCollectors;

            StageCollectors* collectors = pipe.createStage<StageCollectors>();
            collectors->createOperators(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        Pipe& parallel(int no_workers = 1) {
            pipe.setNbWorkers(no_workers);

            return *this;
        };


        template < typename T,
                   template < typename ELEM, class ALLOC = std::allocator<ELEM>>
                   class TContainer >
        TContainer<T> collect() {
            typedef CollectorOperator<T, TContainer<T>> Collector;
            typedef Collectors<Collector> StageCollectors;

            StageCollectors* collectors = pipe.createStage<StageCollectors>();
            collectors->createOperators(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

    template < typename In, typename Out >
    Pipe& map(std::function< Out*(In*) > const& taskFunc) {
        typedef MapOperator<In, Out> Map;
        typedef BaseStage<Map> Stage;

        Stage* stage = pipe.createStage<Stage>();
        stage->createOperators(pipe.nbWorkers(), taskFunc);
        pipe.addStage(stage);

        return *this;
    }

    template < typename In >
    Pipe& find(std::function< bool(In*) > const& taskFunc) {
        typedef FindOperator<In> Find;
        typedef BaseStage<Find> Stage;

        Stage* stage = pipe.createStage<Stage>();
        stage->createOperators(pipe.nbWorkers(), taskFunc);
        pipe.addStage(stage);

        return *this;
    }

    template< typename In, typename Out, typename OutContainer >
    Pipe& flatMap(std::function< OutContainer*(In*) > const& taskFunc) {
        typedef MapOperator<In, OutContainer> Map;
        typedef FlatOperator<OutContainer, Out> Flat;
        typedef BaseStage<Map> MapStage;
        typedef BaseStage<Flat> FlatStage;

        MapStage* mapStage = pipe.createStage<MapStage>();
        FlatStage* flatStage = pipe.createStage<FlatStage>();

        mapStage->createOperators(pipe.nbWorkers(), taskFunc);
        flatStage->createOperators(pipe.nbWorkers());

        pipe.addStage(mapStage);
        pipe.addStage(flatStage);

        return *this;
    };

    template< typename In, typename Out, typename OutContainer = In >
    Pipe& flatMap() {
        typedef FlatOperator<In, Out> Flat;
        typedef BaseStage<Flat> Stage;

        Stage* stage = pipe.createStage<Stage>();
        stage->createOperators(pipe.nbWorkers());
        pipe.addStage(stage);

        return *this;
    };

    template < typename In >
    Pipe& peek(std::function< void(In*) > const& taskFunc) {
        typedef PeekOperator<In> Peek;
        typedef BaseStage<Peek> Stage;

        Stage* stage = pipe.createStage<Stage>();
        stage->createOperators(pipe.nbWorkers(), taskFunc);
        pipe.addStage(stage);

        return *this;
    }

    template < typename In, typename Out = In >
    Out reduce(Reducer<In, Out> const& reducer) {
        typedef ReduceOperator<In, Out> Reduce;
        typedef Collectors<Reduce> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), reducer);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename In, typename Out = In >
    Out reduce(std::function<Out (In, Out)> accumulator) {
        // Cas simple special ou accumulator = combiner & pas de valeur initiale.
        typedef ReduceOperator<In, Out> Reduce;
        typedef Collectors<Reduce> StageCollectors;

        Reducer<In, Out> reducer(accumulator, accumulator);
        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), reducer);
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
        typedef Collectors<GroupByKey> StageCollectors;
                
        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), taskFuncOnKey);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename In, typename K = In, typename V = In,
               typename MapType = std::unordered_map<K, std::vector<V>> >
    MapType groupByKey(std::function<K*(In*)> const& taskFuncOnKey, std::function<V*(In*)> const& taskFuncOnValue) {
        typedef GroupByKeyOperator<In, K, V, MapType, true> GroupByKey;
        typedef Collectors<GroupByKey> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), taskFuncOnKey, taskFuncOnValue);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename In, typename K = In, typename V = In,
               typename MapType = std::unordered_map<K, V> >
    MapType reduceByKey(Reducer<In, V> const& reducer) {
        typedef ReduceByKeyOperator<In, K, V, MapType, false> ReduceByKey;
        typedef Collectors<ReduceByKey> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), reducer);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename In, typename K = In, typename V = In,
               typename MapType = std::unordered_map<K, V> >
    MapType reduceByKey(std::function<K*(In*)> const& taskFuncOnKey, Reducer<In, V> const& reducer) {
        typedef ReduceByKeyOperator<In, K, V, MapType, true> ReduceByKey;
        typedef Collectors<ReduceByKey> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), taskFuncOnKey, reducer);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename In, typename K = In, typename V = In,
               typename MapType = std::unordered_map<K, V> >
    MapType reduceByKey2(std::function< void(V*, In*) > const& accumulator, std::function< void(V*, V*) > const& combiner) {
        typedef ReduceByKeyOperator2<In, K, V, MapType> ReduceByKey;
        typedef Collectors<ReduceByKey> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), accumulator, combiner);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename T >
    T min(std::function< void(T*, T*) > compare = ([](T* a, T* b) { if (*a> *b) *a = *b;})) {
        typedef MinOperator<T> Min;
        typedef Collectors<Min> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), compare);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename T >
        T max(std::function< void(T*, T*) > compare = ([](T* a, T* b) { if (*a <*b) *a = *b;})) {
        typedef MaxOperator<T> Max;
        typedef Collectors<Max> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), compare);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename T >
        bool anyMatch(std::function< bool(T*) > predicate) {
        typedef AnyMatchOperator<T> AnyMatch;
        typedef Collectors<AnyMatch> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), predicate);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }

    template < typename T >
        bool noneMatch(std::function< bool(T*) > predicate) {
        typedef NoneMatchOperator<T> NoneMatch;
        typedef Collectors<NoneMatch> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), predicate);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }


    template < typename T >
        bool allMatch(std::function< bool(T*) > predicate) {
        typedef AllMatchOperator<T> AllMatch;
        typedef Collectors<AllMatch> StageCollectors;

        StageCollectors* collectors = pipe.createStage<StageCollectors>();
        collectors->createOperators(pipe.nbWorkers(), predicate);
        pipe.addStage(collectors);
        pipe.run();

        return collectors->value();
    }


private:
    Pipeline pipe;
};

}
