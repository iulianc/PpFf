
#include <operators/CountOperator.hpp>
#include <operators/SumOperator.hpp>
#include <stages/Source.hpp>
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
#include <collections/Collection.hpp>
#include <stages/Stage.hpp>
#include <stages/Collectors.hpp>
#include <stages/BaseStage.hpp>
#include <utilities/Debug.hpp>
#include <utilities/Identity.hpp>
#include <vector>
#include <map>
#include <unordered_map>

#include <operators/ReduceByKeyOperator2.hpp>
#include <pipeline/PipeManager.hpp>

using namespace PpFf;

namespace PpFf {

    class Flow {
    public:
        Flow() {};

        ~Flow() {};

        Flow& parallel(int no_workers = 1) {
            pipe.setNbWorkers(no_workers);

            return *this;
        };

        static Flow& source(const std::string& path) {
            Flow* pipe = new Flow();
            typedef LinesFromFileOperator LinesFromFile;

            Source<LinesFromFile>* stage = new Source<LinesFromFile>();
            stage->addOperator(pipe->pipe.nbWorkers(), path);
            pipe->pipe.addStage(stage);

            return *pipe;
        }

        template < typename T, typename Iterator >
        static Flow& source(Iterator begin, Iterator end) {
            Flow* pipe = new Flow();
            typedef SourceOperator<T, Iterator> SourceOp;

            Source<SourceOp>* stage = new Source<SourceOp>();
            stage->addOperator(pipe->pipe.nbWorkers(), begin, end);
            pipe->pipe.addStage(stage);

            return *pipe;
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
                   template <typename ELEM, class ALLOC = std::allocator<ELEM>>
                   class TContainer >
            TContainer<T> collect() {
            typedef CollectorOperator<T, TContainer<T>> Collector;

            Collectors<Collector>* collectors = new Collectors<Collector>();
            collectors->addOperator(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T,
                   template <typename ELEM, class ALLOC = std::allocator<ELEM>>
                   class TContainer >
            Collection<T, TContainer, Flow> intermediateCollect() {
            typedef CollectorOperator<T, TContainer<T>> Collector;
            Collectors<Collector>* collectors = new Collectors<Collector>();
            collectors->addOperator(pipe.nbWorkers());
            pipe.addStage(collectors);
            pipe.run();

            // Je trouve louche les deux instructions qui suivent --
            // si je comprends bien le modele d'allocation memoire de
            // C++.  L'objet Collection que tu definis est alloue sur
            // la pile. Tu retournes ensuite cet objet comme resultat
            // de la fonction.  Mais comme il a ete alloue sur la
            // pile, il pourrait ulterieurement etre ecrase, non?

            Collection<T, TContainer, Flow> Collection(collectors->value());
            return Collection;
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

        template< typename In, typename Out, typename OutContainer >
        Flow& flatMap(std::function<OutContainer*(In*)> const& taskFunc) {
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
        Flow& flatMap() {
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

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, std::vector<V>> >
            MapType groupByKey(std::function<K*(In*)> const& keyFunction,
                               std::function<V*(In*)> const& valueFunction = identity<In,V>) {
            typedef GroupByKeyOperator<In, K, V, MapType> GroupByKey;
            
            Collectors<GroupByKey>* collectors = new Collectors<GroupByKey>();
            collectors->addOperator(pipe.nbWorkers(), keyFunction, valueFunction);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, V> >
        MapType reduceByKey(Reducer<In, V> const& reducer,
                            std::function<K*(In*)> const& keyFunction = identity<In,K>) {
            typedef ReduceByKeyOperator<In, K, V, MapType> ReduceByKey;
            
            Collectors<ReduceByKey>* collectors = new Collectors<ReduceByKey>();
            collectors->addOperator(pipe.nbWorkers(), keyFunction, reducer);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        T min(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a > *b) *a = *b; })) {
            typedef MinOperator<T> Min;
            
            Collectors<Min>* collectors = new Collectors<Min>();
            collectors->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        T max(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a < *b) *a = *b; })) {
            typedef MaxOperator<T> Max;
            
            Collectors<Max>* collectors = new Collectors<Max>();
            collectors->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        bool anyMatch(std::function<bool(T*)> predicate) {
            typedef AnyMatchOperator<T> AnyMatch;
            
            Collectors<AnyMatch>* collectors = new Collectors<AnyMatch>();
            collectors->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }

        template < typename T >
        bool noneMatch(std::function<bool(T*)> predicate) {
            typedef NoneMatchOperator<T> NoneMatch;
            
            Collectors<NoneMatch>* collectors = new Collectors<NoneMatch>();
            collectors->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
        }


        template < typename T >
        bool allMatch(std::function<bool(T*)> predicate) {
            typedef AllMatchOperator<T> AllMatch;
            
            Collectors<AllMatch>* collectors = new Collectors<AllMatch>();
            collectors->addOperator(pipe.nbWorkers(), predicate);
            pipe.addStage(collectors);
            pipe.run();

            return collectors->value();
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

            Collectors<Sort>* collectors = new Collectors<Sort>();
            collectors->addOperator(pipe.nbWorkers(), compare);
            pipe.addStage(collectors);
            pipe.run();

            Collection< T, std::vector, Flow > Collection(collectors->value());
            return Collection;
        }
		
        void consume(void) {
            pipe.run();
        }		
    
    private:
        PipeManager pipe;
    };
    
}