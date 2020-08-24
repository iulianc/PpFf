#ifndef SUMOPERATOR_HPP
#define SUMOPERATOR_HPP

#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename T >
    class SumOperator: public Operator {
    public:
        typedef T Value;
        SumOperator()
        { operatorType = COLLECTOR_OP; }

        SumOperator(const SumOperator& other) : sum(other.sum)
        { operatorType = COLLECTOR_OP; }

        SumOperator(SumOperator&& other) noexcept : sum(std::move(other.sum))
        { operatorType = COLLECTOR_OP; }

        SumOperator& operator+=(const SumOperator& other) {
            sum += other.sum;

            return *this ;
        }

        virtual ~SumOperator() {}

        void* svc(void* task) {
            sum += *((T*)task);

            return GO_ON;
        }

        T value() {
            return sum;
        }

    private:
        T sum{};
    };

}

#endif
