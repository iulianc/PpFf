#ifndef REDUCEBYKEYOPERATOR2_HPP
#define REDUCEBYKEYOPERATOR2_HPP

#include <operators/FinalOperator.hpp>
#include <functional>

namespace PpFf {

    template < typename In, typename K, typename V, typename MapContainer >
    class ReduceByKeyOperator2: public FinalOperator {
    public:
        typedef MapContainer Value;

        ReduceByKeyOperator2(std::function< void(V*, In*) > const& accumulator, std::function< void(V*, V*) > const& combiner): accumulator(accumulator), combiner(combiner) { }

        ReduceByKeyOperator2(const ReduceByKeyOperator2& other) : accumulator(other.accumulator), combiner(other.combiner) { }

        ReduceByKeyOperator2(ReduceByKeyOperator2&& other) noexcept : accumulator(std::move(other.accumulator)), combiner(std::move(other.combiner)) { }

        ReduceByKeyOperator2& operator+= (ReduceByKeyOperator2& other) {
            for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
                mapIt = mapContainer.find(otherIt->first);
                
                if (mapIt != mapContainer.end()) {
                    combiner(&(mapIt->second), &(otherIt->second));
                } else {
                    mapContainer[otherIt->first] = otherIt->second;
                }
            }

            return *this;
        }

        ~ReduceByKeyOperator2() { };

        void* svc(void* task) {
            accumulator(&(mapContainer[*((In*)task)]), (In*)task);

            return GO_ON;
        }

        MapContainer value() {
            return mapContainer;
        }

    private:
        std::function<void(V*, In*)> const& accumulator;
        std::function<void(V*, V*)> const& combiner;
        V val{ };
        MapContainer mapContainer;
        typename MapContainer::iterator mapIt;
    };

}

#endif
