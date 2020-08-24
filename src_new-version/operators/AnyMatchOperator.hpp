#ifndef ANYMACHOPERATOR_HPP
#define ANYMACHOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>
#include <operators/Operator.hpp>

namespace PpFf{

    template < typename T >
    class AnyMatchOperator: public Operator {
    public:
        typedef bool Value;

        AnyMatchOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc)
        { operatorType = COLLECTOR_OP; }

        AnyMatchOperator(const AnyMatchOperator& other) : taskFunc(other.taskFunc)
        { operatorType = COLLECTOR_OP; }

        AnyMatchOperator(AnyMatchOperator&& other) noexcept : taskFunc(std::move(other.taskFunc))
        { operatorType = COLLECTOR_OP; }

        AnyMatchOperator& operator+= ( const AnyMatchOperator& other ) {
            if (!val){
                val = other.val;
            }
            return *this;
        }

        ~AnyMatchOperator()
        {}

        void* svc(void* task) {
            if (val)
                return EOS;

            val = taskFunc((T*)task);

            return (T*)GO_ON;
        }

        bool value() {
            return val;
        }

    private:
        std::function< bool(T*) > const& taskFunc;
        bool val = false;
    };

}

#endif
