#ifndef REDUCEOPERATOR_HPP
#define REDUCEOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <operators/Reducer.hpp>


namespace PpFf {

    template < typename In, typename Out >
    class ReduceOperator: public FinalOperator {
    public:
        typedef Out Value;

        // Constructeur pas utilise... et interface maintenant incorrecte!
        //        ReduceOperator(std::function< void(Out*, In*) > const& accumulator) 
        //        { reducer.accumulator = accumulator; }

        ReduceOperator(Reducer<In, Out> const& reducer): reducer(reducer)
        {}

        ReduceOperator(const ReduceOperator& other): reducer(other.reducer)
        {}

        ReduceOperator(ReduceOperator&& other) noexcept: reducer(std::move(other.reducer))
        {}

        ReduceOperator& operator+=(ReduceOperator& other) {
            if (!reducer.hasCombiner) throw std::logic_error( "ReduceOperator::operator+= => There is no combiner!" );
            
            val = reducer.combiner(val, other.val);

            return *this ;
        }

        ~ReduceOperator() {};
        
        void* svc(void* task) {
            if (initialized) {
                val = reducer.accumulator(val, *((In*)task));
            } else {
                // Pas initialise: On utilise le premier element
                // rencontre comme valeur initiale de l'accumulateur.
                val = *((Out*)task);
                initialized = true;
            }

            return (Out*)GO_ON;
        }

        Out value() {
            return val;
        }

    private:
        Reducer<In, Out> const& reducer;
        bool initialized = reducer.hasInitialValue;
        Out val = reducer.initialValue;
    };

}

#endif
