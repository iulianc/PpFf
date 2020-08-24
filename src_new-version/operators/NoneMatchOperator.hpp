#ifndef NONEMACHOPERATOR_HPP
#define NONEMACHOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename T >
    class NoneMatchOperator: public Operator {
    public:
        typedef bool Value;

        NoneMatchOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc)
        { operatorType = COLLECTOR_OP; }

        NoneMatchOperator(const NoneMatchOperator& other) : taskFunc(other.taskFunc)
        { operatorType = COLLECTOR_OP; }

        NoneMatchOperator(NoneMatchOperator&& other) noexcept : taskFunc(std::move(other.taskFunc))
        { operatorType = COLLECTOR_OP; }

        NoneMatchOperator& operator+= ( const NoneMatchOperator& other ) {
            if (val) {
                val = other.val;
            }
            return *this;
        }

        ~NoneMatchOperator()
        {}

        void* svc(void* task) {
            val = !(taskFunc((T*)task)) and val;

            return (T*)GO_ON;
        }

        bool value(){
            return val;
        }


    private:
        std::function< bool(T*) > const& taskFunc;
        bool val = true;
    };

}

#endif
