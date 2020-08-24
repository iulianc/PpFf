#ifndef COUNTOPERATOR_HPP
#define COUNTOPERATOR_HPP

#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename T >
    class CountOperator: public Operator {
    public:
        typedef T Value;

        CountOperator()
        { operatorType = COLLECTOR_OP; }

        CountOperator(const CountOperator& other) : counter(other.counter)
        { operatorType = COLLECTOR_OP; }

        CountOperator(CountOperator&& other) noexcept : counter(std::move(other.counter))
        { operatorType = COLLECTOR_OP; }

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
