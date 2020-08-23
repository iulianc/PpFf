#ifndef MINOPERATOR_HPP
#define MINOPERATOR_HPP

#include <ff/ff.hpp>
#include <operators/Reducer.hpp>
#include <functional>

namespace PpFf {

    template < typename T >
    class MinOperator: public ff::ff_node {
    public:
        typedef T Value;

        MinOperator(std::function< void(T*, T*) > compare): compare(compare)
        {}

        MinOperator(const MinOperator& other) : compare(other.compare)
        {}

        MinOperator(MinOperator&& other) noexcept : compare(std::move(other.compare))
        {}

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
