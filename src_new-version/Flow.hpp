
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

        ////////////////////////////////////////////////
        // Methodes (statiques) pour creer un Flow.
        ////////////////////////////////////////////////
        static Flow& source(const std::string& path) {
            typedef LinesFromFileOperator LinesFromFile;

            Flow* flow = new Flow();
            flow->pipe.addNodes<LinesFromFile>(1, path);

            return *flow;
        }

        template < typename T, typename Iterator >
        static Flow& source(Iterator begin, Iterator end) {
            typedef SourceOperator<T, Iterator> SourceOp;
            
            Flow* flow = new Flow();
            flow->pipe.addNodes<SourceOp>(1, begin, end);

            return *flow;
        }


        ////////////////////////////////////////////////
        // Methode pour modifier le niveau de parallelisme.
        ////////////////////////////////////////////////
        Flow& parallel(int no_workers = 1) {
            pipe.setNbWorkers(no_workers);
            
            return *this;
        };


        ////////////////////////////////////////////////
        // Methodes pour des traitements intermedaires, donc qui produisent un Flow.
        ////////////////////////////////////////////////
        
        template < typename In >
        Flow& find(std::function<bool(In*)> const& taskFunc) {
            typedef FindOperator<In> Find;
            
            pipe.addNodes<Find>(pipe.nbWorkers(), taskFunc);
            
            return *this;
        }

        template< typename In, typename TContainer, typename Out >
        Flow& flatMap(std::function<TContainer*(In*)> const& taskFunc) {
            typedef FlatMapOperator<In, TContainer, Out> FlatMap;
            
            pipe.addNodes<FlatMap>(pipe.nbWorkers(), taskFunc);
            
            return *this;
        };

        template< typename In, typename Out, typename OutContainer = In >
        Flow& flatten() {
            typedef FlatOperator<In, Out> Flat;
            
            pipe.addNodes<Flat>(pipe.nbWorkers());
            
            return *this;
        };

        template < typename T >
        Flow& limit(int n) {
            typedef LimitOperator<T> Limit;
            
            pipe.addNodes<Limit>(pipe.nbWorkers(), n);

            return *this;
        }

        template < typename In, typename Out >
        Flow& map(std::function<Out*(In*)> const& taskFunc) {
            typedef MapOperator<In, Out> Map;
            
            pipe.addNodes<Map>(pipe.nbWorkers(), taskFunc);

            return *this;
        }

        template < typename In >
        Flow& peek(std::function<void(In*)> const& taskFunc) {
            typedef PeekOperator<In> Peek;
            
            pipe.addNodes<Peek>(pipe.nbWorkers(), taskFunc);
            
            return *this;
        }

        template < typename T >
        Flow& skip(int n) {
            typedef SkipOperator<T> Skip;
            
            pipe.addNodes<Skip>(pipe.nbWorkers(), n);
            
            return *this;
        }
        
        ////////////////////////////////////////////////
        // Methodes pour produire un resultat final booleen.
        ////////////////////////////////////////////////
        
        template < typename T >
        bool allMatch(std::function<bool(T*)> predicate) {
            typedef AllMatchOperator<T> AllMatch;

            pipe.addNodes<AllMatch>(pipe.nbWorkers(), predicate);
            pipe.run();

            return pipe.value<AllMatch, bool>();
        }

        template < typename T >
        bool anyMatch(std::function<bool(T*)> predicate) {
            typedef AnyMatchOperator<T> AnyMatch;
            
            pipe.addNodes<AnyMatch>(pipe.nbWorkers(), predicate);
            pipe.run();

            return pipe.value<AnyMatch, bool>();
        }

        template < typename T >
        bool noneMatch(std::function<bool(T*)> predicate) {
            typedef NoneMatchOperator<T> NoneMatch;
            
            pipe.addNodes<NoneMatch>(pipe.nbWorkers(), predicate);
            pipe.run();

            return pipe.value<NoneMatch, bool>();
        }


        ////////////////////////////////////////////////
        // Methodes pour produire un resultat final simple.
        ////////////////////////////////////////////////
        unsigned int count() {
            typedef CountOperator<int> Count;
            
            pipe.addNodes<Count>(pipe.nbWorkers());
            pipe.run();

            return pipe.value<Count, int>();
        }

        template < typename T >
        T max(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a < *b) *a = *b; })) {
            typedef MaxOperator<T> Max;
            
            pipe.addNodes<Max>(pipe.nbWorkers(), compare);
            pipe.run();

            return pipe.value<Max, T>();
        }

        template < typename T >
        T min(std::function<void(T*, T*)> compare = ([](T* a, T* b) { if (*a > *b) *a = *b; })) {
            typedef MinOperator<T> Min;
            
            pipe.addNodes<Min>(pipe.nbWorkers(), compare);
            pipe.run();

            return pipe.value<Min, T>();
        }

        template < typename In, typename Out = In >
        Out reduce(Reducer<In, Out> const& reducer) {
            typedef ReduceOperator<In, Out> Reduce;
            
            pipe.addNodes<Reduce>(pipe.nbWorkers(), reducer);
            pipe.run();

            return pipe.value<Reduce, Out>();
        }

        template < typename In, typename Out = In >
        // Cas special ou accumulator = combiner.
        Out reduce(Out initialValue, std::function<Out (In, Out)> accumulator) {
            typedef ReduceOperator<In, Out> Reduce;

            Reducer<In, Out> reducer(initialValue, accumulator, accumulator);
            
            pipe.addNodes<Reduce>(pipe.nbWorkers(), reducer);
            pipe.run();

            return pipe.value<Reduce, Out>();
        }

        template < typename T >
        T sum() {
            typedef SumOperator<T> Sum;
            
            pipe.addNodes<Sum>(pipe.nbWorkers());
            pipe.run();

            return pipe.value<Sum, T>();
        }

        ////////////////////////////////////////////////
        // Methodes pour produire un resultat final plus complexe.
        ////////////////////////////////////////////////
        
        template < typename T,
                   template <typename ELEM, class ALLOC = std::allocator<ELEM>>
                   class TContainer >
        TContainer<T> collect() {
            typedef CollectorOperator<T, TContainer<T>> TOperator;
            
            pipe.addNodes<TOperator>(pipe.nbWorkers());
            pipe.run();

            return pipe.value<TOperator, TContainer<T>>();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, std::vector<V>> >
        MapType groupByKey(std::function<K*(In*)> const& keyFunction,
                           std::function<V*(In*)> const& valueFunction = identity<In,V>) {
            typedef GroupByKeyOperator<In, K, V, MapType> GroupByKey;

            pipe.addNodes<GroupByKey>(pipe.nbWorkers(),
                                       keyFunction,
                                       valueFunction);
            pipe.run();

            return pipe.value<GroupByKey, MapType>();
        }

        template < typename In, typename K = In, typename V = In,
                   typename MapType = std::unordered_map<K, V> >
        MapType reduceByKey(Reducer<In, V> const& reducer,
                            std::function<K*(In*)> const& keyFunction = identity<In,K>) {
            typedef ReduceByKeyOperator<In, K, V, MapType> ReduceByKey;

            pipe.addNodes<ReduceByKey>(pipe.nbWorkers(), keyFunction, reducer);
            pipe.run();

            return pipe.value<ReduceByKey, MapType>();
        }

        template < typename T >
        Collection<T, std::vector, Flow> sort(std::function<bool(T, T)> const& compare = std::less<T>()) {
            typedef SortOperator<T> Sort;
            typedef Collection<T, std::vector, Flow> TCollection;

            pipe.addNodes<Sort>(pipe.nbWorkers(), compare);
            pipe.run();

            return TCollection(pipe.value<Sort, TCollection>());
        }
		
    private:
        Pipeline pipe;
    };
    
}
