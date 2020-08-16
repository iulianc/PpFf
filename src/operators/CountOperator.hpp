#ifndef COUNTOPERATOR_HPP
#define COUNTOPERATOR_HPP

#include <operators/BaseOperator.hpp>

namespace PpFf {

    template < typename T >
    class CountOperator: public BaseOperator {
    public:
        typedef T Value;

        CountOperator()
        {}

        CountOperator(const CountOperator& other) : counter(other.counter)
        {}

        CountOperator(CountOperator&& other) noexcept : counter(std::move(other.counter))
        {}

        CountOperator& operator+=( const CountOperator& other ) {
            counter += other.counter;
            return *this;
        }

        ~CountOperator()
        {}

        void* svc(void* task) {
            counter++;
            return GO_ON;
        }

        T value(){
            return counter;
        }

    private:
        T counter = 0;
    };

}

#endif
