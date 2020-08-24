#ifndef GROUPBYKEYOPERATOR_HPP
#define GROUPBYKEYOPERATOR_HPP

#include <ff/ff.hpp>
#include <utilities/Identity.hpp>
#include <functional>
#include <operators/CollectorOp.hpp>

namespace PpFf {


    template < typename In, typename K, typename V, typename MapContainer >
    class GroupByKeyOperator: public CollectorOp {
    public:
        typedef MapContainer Value;

        GroupByKeyOperator(std::function< K*(In*) > const& taskFuncOnKey, 
                           std::function< V*(In*) > const& taskFuncOnValue = identity<In,V>) : 
            taskFuncOnKey(taskFuncOnKey),
            taskFuncOnValue(taskFuncOnValue)
        {}

        GroupByKeyOperator(const GroupByKeyOperator& other) : 
            taskFuncOnKey(other.taskFuncOnKey),
            taskFuncOnValue(other.taskFuncOnValue),
            mapContainer(other.mapContainer)
        {}

        GroupByKeyOperator(GroupByKeyOperator&& other) noexcept : 
            taskFuncOnKey(std::move(other.taskFuncOnKey)),
            taskFuncOnValue(std::move(other.taskFuncOnValue)),
            mapContainer(std::move(other.mapContainer))
        {}

        ~GroupByKeyOperator()
        {}

        GroupByKeyOperator& operator+=(const GroupByKeyOperator& other) {
            for (auto otherIt = other.mapContainer.begin(); otherIt != other.mapContainer.end(); otherIt++) {
                auto mapIt = mapContainer.find(otherIt->first);
                
                if (mapIt != mapContainer.end()) {
                    mapIt->second.insert(mapIt->second.end(), otherIt->second.begin(), otherIt->second.end());
                } else {
                    mapContainer[otherIt->first] = otherIt->second;
                }
            }
            return *this ;
        }

        void* svc(void* task) {
            K* key = taskFuncOnKey((In*)task);
            V* val = taskFuncOnValue((In*)task);
            mapContainer[*key].push_back(*val);

            return GO_ON;
        }

        MapContainer value() {
            return mapContainer;
        }

    private:
        std::function< K*(In*) > const& taskFuncOnKey;
        std::function< V*(In*) > const& taskFuncOnValue;
        MapContainer mapContainer;
    };
}

#endif
