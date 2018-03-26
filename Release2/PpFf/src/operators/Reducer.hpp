#ifndef REDUCER_HPP
#define REDUCER_HPP

#include <functional>

namespace PpFf{

    template< typename In, typename Out >
    class Reducer {
    public:
        // Valeur bidon pour supprimer les warnings sur MacBook.
        std::function<void(Out*, Out*)> dummyCombiner = [](Out*, Out*){};

        Out identity{};

        std::function< void(Out*, In*) > const& accumulator;
        std::function< void(Out*, Out*) > const& combiner = dummyCombiner;
        bool const hasCombiner = false;

        Reducer(std::function< void(Out*, In*) > const& accumulator): 
            accumulator(accumulator) {}

        Reducer(std::function< void(Out*, In*) > const& accumulator, 
                std::function< void(Out*, Out*) > const& combiner): 
            accumulator(accumulator), 
            combiner(combiner), 
            hasCombiner(true) {}

        Reducer(Out identity, 
                std::function< void(Out*, In*) > const& accumulator):
            identity(identity), 
            accumulator(accumulator) {}

        Reducer(Out identity, 
                std::function< void(Out*, In*) > const& accumulator, 
                std::function< void(Out*, Out*) > const& combiner):
            identity(identity), 
            accumulator(accumulator), 
            combiner(combiner), 
            hasCombiner(true) {}

        ~Reducer() {}
    };

}

#endif
