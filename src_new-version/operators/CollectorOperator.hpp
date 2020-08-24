#ifndef COLLECTOROPERATOR_HPP
#define COLLECTOROPERATOR_HPP

#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename T, typename TContainer >
    class CollectorOperator: public Operator {
    public:
        typedef TContainer Value;

        CollectorOperator()
        { operatorType = COLLECTOR_OP; }

        CollectorOperator(const CollectorOperator& other): container(other.container)
        { operatorType = COLLECTOR_OP; }

        CollectorOperator(CollectorOperator&& other) noexcept: container(std::move(other.container))
        { operatorType = COLLECTOR_OP; }

        CollectorOperator& operator+=(const CollectorOperator& other) {
            for (T elem: other.container) {
                container.push_back(elem);
            }

            return *this ;
        }

        ~CollectorOperator() 
        {}

        void* svc(void* task) {
            container.push_back(*((T*)task));

            return (T*)GO_ON;
        }

        TContainer value() {
            return container;
        }

    private:
        TContainer container{};
    };

}

#endif
