#ifndef REDUCEBYKEYOPERATOR_HPP
#define REDUCEBYKEYOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <operators/Reducer.hpp>
#include <functional>

namespace PpFf{

    template < typename In, typename K, typename V, typename MapContainer, bool IsFuncOnKey >
    class ReduceByKeyOperator: public FinalOperator {};


    template < typename In, typename K, typename V, typename MapContainer >
    class ReduceByKeyOperator<In, K, V, MapContainer, false>: public FinalOperator {
    public:
        typedef MapContainer Value;
        ReduceByKeyOperator(Reducer<In, V> const& reducer): reducer(reducer) { }
        ReduceByKeyOperator(const ReduceByKeyOperator& other): reducer(other.reducer) { }
        ReduceByKeyOperator(ReduceByKeyOperator&& other) noexcept: reducer(std::move(other.reducer)) { }
        ReduceByKeyOperator& operator+= (ReduceByKeyOperator& other) {
            for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
                mapIt = mapContainer.find(otherIt->first);

                if (mapIt != mapContainer.end()) {
                    if (reducer.hasCombiner) {
                        mapIt->second = reducer.combiner(mapIt->second, otherIt->second);
                    }
                } else {
                    mapContainer[otherIt->first] = otherIt->second;
                }
            }

            return *this;
        }

        ~ReduceByKeyOperator() {};

        void* svc(void* task) {
            mapIt = mapContainer.find(*((In*)task));

            if (mapIt != mapContainer.end()) {
                mapIt->second = reducer.accumulator(mapIt->second, *((In*)task));
            } else {
                mapContainer[*((In*)task)] = val;
                mapContainer[*((In*)task)] = reducer.accumulator(mapContainer[*((In*)task)], *((In*)task));
            }

            return GO_ON;
        }

        MapContainer value() {
            return mapContainer;
        }

    private:
        Reducer<In, V> const& reducer;
        V val = reducer.initialValue;
        MapContainer mapContainer;
        typename MapContainer::iterator mapIt;
    };



    template < typename In, typename K, typename V, typename MapContainer >
    class ReduceByKeyOperator<In, K, V, MapContainer, true>: public FinalOperator {
    public:
        typedef MapContainer Value;
        ReduceByKeyOperator(std::function<K*(In*)> const& taskFuncOnKey, Reducer<In, V> const& reducer): taskFuncOnKey(taskFuncOnKey), reducer(reducer) { }
        ReduceByKeyOperator(const ReduceByKeyOperator& other): taskFuncOnKey(other.taskFuncOnKey), reducer(other.reducer) { }
        ReduceByKeyOperator(ReduceByKeyOperator&& other) noexcept: taskFuncOnKey(std::move(other.taskFuncOnKey)), reducer(std::move(other.reducer)) { }
        ReduceByKeyOperator& operator+= (ReduceByKeyOperator& other) {
            for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
                mapIt = mapContainer.find(otherIt->first);

                if (mapIt != mapContainer.end()) {
                    if (reducer.hasCombiner) {
                        mapIt->second = reducer.combiner(mapIt->second, otherIt->second);
                    }
                } else {
                    mapContainer[otherIt->first] = otherIt->second;
                }
            }

            return *this;
        }

        ~ReduceByKeyOperator() { };

        void* svc(void* task) {
            key = taskFuncOnKey((In*)task);
            mapIt = mapContainer.find(*key);

            if (mapIt != mapContainer.end()) {
                mapIt->second = reducer.accumulator(mapIt->second, *((In*)task));
            } else {
                mapContainer[*key] = val;
                mapContainer[*key] = reducer.accumulator(mapContainer[*key], *((In*)task));
            }

            return GO_ON;
        }

        MapContainer value() {
            return mapContainer;
        }

    private:
        std::function<K*(In*)> const& taskFuncOnKey;
        Reducer<In, V> const& reducer;
        V val = reducer.initialValue;
        MapContainer mapContainer;
        typename MapContainer::iterator mapIt;
        K *key;
    };

}

#endif
