#ifndef SUMOPERATOR_HPP
#define SUMOPERATOR_HPP

#include <ff/ff.hpp>
#include <operators/CollectorOp.hpp>

namespace PpFf {

    template < typename T >
    class SumOperator: public CollectorOp {
    public:
        typedef T Value;
        SumOperator()
        {}

        SumOperator(const SumOperator& other) : sum(other.sum)
        {}

        SumOperator(SumOperator&& other) noexcept : sum(std::move(other.sum))
        {}

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
