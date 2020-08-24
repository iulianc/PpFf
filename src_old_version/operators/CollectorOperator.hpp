#ifndef COLLECTOROPERATOR_HPP
#define COLLECTOROPERATOR_HPP

#include <operators/BaseOperator.hpp>

using namespace ff;

namespace PpFf {

    template < typename T, typename TContainer >
    class CollectorOperator: public BaseOperator {
    public:
        typedef TContainer Value;

        CollectorOperator()
        {}

        CollectorOperator(const CollectorOperator& other): container(other.container)
        {}

        CollectorOperator(CollectorOperator&& other) noexcept: container(std::move(other.container))
        {}

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
