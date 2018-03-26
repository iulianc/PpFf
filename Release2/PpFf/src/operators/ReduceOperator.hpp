#ifndef REDUCEOPERATOR_HPP
#define REDUCEOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <operators/Reducer.hpp>


namespace PpFf {

    template < typename In, typename Out >
    class ReduceOperator: public FinalOperator {
    public:
        typedef Out Value;

        ReduceOperator(std::function< void(Out*, In*) > const& accumulator) 
        { reducer.accumulator = accumulator; }

        ReduceOperator(Reducer<In, Out> const& reducer): 
            reducer(reducer) {};

        ReduceOperator(const ReduceOperator& other): 
            reducer(other.reducer) {}

        ReduceOperator(ReduceOperator&& other) noexcept: 
            reducer(std::move(other.reducer)) {}

        ReduceOperator& operator+= ( ReduceOperator& other ) {
            if (reducer.hasCombiner) {
                reducer.combiner(&val, &other.val);
            }
            return *this ;
        }

        ~ReduceOperator() {};

        void* svc(void* task) {
            reducer.accumulator(&val, (In*)task);

            return (Out*)GO_ON;
        }

        Out value() {
            return val;
        }

    private:
        Reducer<In, Out> const& reducer;
        Out val = reducer.identity;
    };

}

#endif
