#ifndef ALLMACHOPERATOR_HPP
#define ALLMACHOPERATOR_HPP

#include <operators/FinalOperator.hpp>
#include <functional>


namespace PpFf {

    template < typename T >
    class AllMatchOperator: public FinalOperator {
    public:
        typedef bool Value;

        AllMatchOperator(std::function< bool(T*) > const& taskFunc): taskFunc(taskFunc) {};

        AllMatchOperator(const AllMatchOperator& other): taskFunc(other.taskFunc) {}

        AllMatchOperator(AllMatchOperator&& other) noexcept: taskFunc(std::move(other.taskFunc)) {}

        AllMatchOperator& operator+= (const AllMatchOperator& other) {
            if (val) {
                val = other.val;
            }
            return *this;
        }

        ~AllMatchOperator() { }

        void* svc(void* task) {
            if (!val)
                return EOS;

            val = (taskFunc((T*)task));

            return (T*)GO_ON;
        }

        bool value() {
            return val;
        }


    private:
        std::function< bool(T*) > const& taskFunc;
        bool val = true;
    };

}

#endif
