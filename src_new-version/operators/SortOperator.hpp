#ifndef SORTOPERATOR_HPP
#define SORTOPERATOR_HPP

#include <functional>
#include <algorithm>
#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {
    
    template< typename T >
    class SortOperator: public Operator {
    public:
    	typedef std::vector<T> Value;

    	SortOperator(std::function<bool(T, T)> const& compare = std::less<T>()): compare(compare)
        { operatorType = COLLECTOR_OP; }

    	SortOperator(SortOperator& other): compare(other.compare)
        { operatorType = COLLECTOR_OP; }

    	SortOperator(SortOperator&& other) noexcept: compare(std::move(other.compare))
        { operatorType = COLLECTOR_OP; }

    	SortOperator& operator+=(const SortOperator& other) {
            container.insert(container.end(), other.container.begin(), other.container.end());

            return *this;
        }
        
        ~SortOperator()
        {}

        void* svc(void* task) {
            container.push_back(*((T*)task));

            return (T*)GO_ON;
        }

        std::vector< T > value() {
            std::sort(container.begin(), container.end(), compare);

            return container;
        }

    private:
        std::function<bool(T, T)> const& compare;
        std::vector<T> container{};
    };

}

#endif
