#ifndef MINOPERATOR_HPP
#define MINOPERATOR_HPP

#include <ff/ff.hpp>
#include <operators/Reducer.hpp>
#include <functional>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename T >
    class MinOperator: public Operator {
    public:
        typedef T Value;

        MinOperator(std::function< void(T*, T*) > compare): compare(compare)
        { operatorType = COLLECTOR_OP; }

        MinOperator(const MinOperator& other) : compare(other.compare)
        { operatorType = COLLECTOR_OP; }

        MinOperator(MinOperator&& other) noexcept : compare(std::move(other.compare))
        { operatorType = COLLECTOR_OP; }

        MinOperator& operator+=(MinOperator& other) {
            compare(val, other.val);

            return *this ;
        }

        ~MinOperator() {
            delete val;
        }

        void* svc(void* task) {
            if (firstVal) {
                val = (T*)task;
                firstVal = false;
            }
            compare(val, (T*)task);

            return (T*)GO_ON;
        }

        T value() {
            return *val;
        }

    private:
        std::function< void(T*, T*) > compare;
        T *val = new T();
        bool firstVal = true;
    };

}

#endif
