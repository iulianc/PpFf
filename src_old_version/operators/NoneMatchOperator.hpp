#ifndef NONEMACHOPERATOR_HPP
#define NONEMACHOPERATOR_HPP

#include <operators/BaseOperator.hpp>
#include <functional>


namespace PpFf{

    template < typename T >
    class NoneMatchOperator: public BaseOperator {
    public:
        typedef bool Value;

        NoneMatchOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc)
        {}

        NoneMatchOperator(const NoneMatchOperator& other) : taskFunc(other.taskFunc)
        {}

        NoneMatchOperator(NoneMatchOperator&& other) noexcept : taskFunc(std::move(other.taskFunc))
        {}

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
