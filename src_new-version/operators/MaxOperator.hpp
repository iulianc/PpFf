#ifndef MAXOPERATOR_HPP
#define MAXOPERATOR_HPP

#include <ff/ff.hpp>
#include <operators/Reducer.hpp>
#include <functional>

namespace PpFf {

    template < typename T >
    class MaxOperator: public ff::ff_node {
    public:
        typedef T Value;

        MaxOperator(std::function< void(T*, T*) > compare): compare(compare)
        {}

        MaxOperator(const MaxOperator& other) : compare(other.compare)
        {}

        MaxOperator(MaxOperator&& other) noexcept : compare(std::move(other.compare))
        {}

        MaxOperator& operator+=( MaxOperator& other ) {
            compare(val, other.val);

            return *this ;
        }

        ~MaxOperator() {
            delete val;
        }

        void* svc(void* task) {
            if (firstVal) {
                val = (T*) task;
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
