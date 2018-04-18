#ifndef SORTOPERATOR_HPP
#define SORTOPERATOR_HPP

#include <functional>
#include <algorithm>
#include <operators/FinalOperator.hpp>

using namespace ff;

namespace PpFf {
    
    template < typename T, bool HasComparator >
    class SortOperator: public FinalOperator {};

    template< typename T >
    class SortOperator<T, false>: public FinalOperator {
    public:
    	typedef std::vector<T> Value;

    	SortOperator() {}

    	SortOperator& operator+=(const SortOperator& other) {
            container.insert(container.end(), other.container.begin(), other.container.end());
            return *this;
        }

        virtual ~SortOperator() {}

        void* svc(void* task) {
            container.push_back(*((T*)task));

            return (T*)GO_ON;
        }

        std::vector<T> value() {
            std::sort (container.begin(), container.end());

            return container;
        }

    private:
        std::vector<T> container{};
    };


    template< typename T >
    class SortOperator<T, true>: public FinalOperator {
    public:
    	typedef std::vector<T> Value;

    	SortOperator(std::function<bool(T, T)> const& compare): compare(compare) { }

    	SortOperator(SortOperator& other): compare(other.compare) { }

    	SortOperator(SortOperator&& other) noexcept: compare(std::move(other.compare)) { }

    	SortOperator& operator+=(const SortOperator& other) {
            container.insert(container.end(), other.container.begin(), other.container.end());

            return *this;
        }

        virtual ~SortOperator() {}

        void* svc(void* task) {
            container.push_back(*((T*)task));

            return (T*)GO_ON;
        }

        std::vector<T> value() {
            std::sort (container.begin(), container.end(), compare);

            return container;
        }

    private:
        std::function<bool(T, T)> const& compare;
        std::vector<T> container{};
    };

}

#endif
