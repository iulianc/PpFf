#ifndef REDUCER_HPP
#define REDUCER_HPP

#include <functional>

namespace PpFf {

    template< typename In, typename Out >
    class Reducer {
    public:
        // Valeur bidon pour supprimer les warnings sur MacBook.
        std::function<Out(Out, Out)> dummyCombiner = [](Out, Out) { Out out{}; return out; };

        Out initialValue{};
        bool const hasInitialValue = false;

        std::function<Out(Out, In)> const& accumulator;

        std::function<Out(Out, Out)> const& combiner = dummyCombiner;
        bool const hasCombiner = false;

        
        // Forme la plus generale, avec les trois elements.
        Reducer(Out initialValue, 
                std::function<Out(Out, In)> const& accumulator,
                std::function<Out(Out, Out)> const& combiner):
            initialValue(initialValue), 
            hasInitialValue(true),
            accumulator(accumulator), 
            combiner(combiner), 
            hasCombiner(true) {}

        Reducer(Out initialValue, 
                std::function<Out(Out, In)> const& accumulator):
            initialValue(initialValue), 
            hasInitialValue(true),
            accumulator(accumulator) {}

        Reducer(std::function<Out(Out, In)> const& accumulator):
            hasInitialValue(false),
            accumulator(accumulator) {}

        Reducer(std::function<Out(Out, In)> const& accumulator,
                std::function<Out(Out, Out)> const& combiner):
            hasInitialValue(false),
            accumulator(accumulator), 
            combiner(combiner), 
            hasCombiner(true) {}

        ~Reducer() {}
    };

}

#endif
