#ifndef MAPOPERATOR_HPP
#define MAPOPERATOR_HPP

#include <functional>
#include <operators/BaseOperator.hpp>


namespace PpFf {

    template < typename In, typename Out >
    class MapOperator: public BaseOperator {
    public:
        MapOperator(std::function< Out*(In*) > const& taskFunc): taskFunc(taskFunc) {}

        MapOperator(const MapOperator& other) : taskFunc(other.taskFunc) {}

        MapOperator(MapOperator&& other) noexcept : taskFunc(std::move(other.taskFunc)) {}

        ~MapOperator() {}

        void* svc(void* task) {
            return taskFunc((In*)task);
        }

    private:
        std::function< Out*(In*) > const& taskFunc;
    };

}

#endif
