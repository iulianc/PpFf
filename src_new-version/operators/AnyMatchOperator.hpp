#ifndef ANYMACHOPERATOR_HPP
#define ANYMACHOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>


namespace PpFf{

    template < typename T >
    class AnyMatchOperator: public ff::ff_node {
    public:
        typedef bool Value;

        AnyMatchOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc)
        {}

        AnyMatchOperator(const AnyMatchOperator& other) : taskFunc(other.taskFunc)
        {}

        AnyMatchOperator(AnyMatchOperator&& other) noexcept : taskFunc(std::move(other.taskFunc))
        {}

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
