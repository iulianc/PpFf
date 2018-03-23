#ifndef REDUCER_HPP
#define REDUCER_HPP

#include <functional>

namespace PpFf{

    template< typename In, typename Out >
    class Reducer {
    public:
        Out identity{};

        std::function< void(Out*, In*) > const& accumulator;
        std::function< void(Out*, Out*) > const& combiner;
        bool const isCombiner;

        Reducer(std::function< void(Out*, In*) > const& accumulator): 
            accumulator(accumulator), combiner([](Out*, Out*) {}), isCombiner(false) {}
        Reducer(std::function< void(Out*, In*) > const& accumulator, std::function< void(Out*, Out*) > const& combiner): 
            accumulator(accumulator), combiner(combiner), isCombiner(true) {}
        Reducer(Out identity, std::function< void(Out*, In*) > const& accumulator):
            identity(identity), accumulator(accumulator), combiner([](Out*, Out*) {}), isCombiner(false) {}
        Reducer(Out identity, std::function< void(Out*, In*) > const& accumulator, std::function< void(Out*, Out*) > const& combiner):
            identity(identity), accumulator(accumulator), combiner(combiner), isCombiner(true) {}

        ~Reducer() { }
    };

}

#endif
