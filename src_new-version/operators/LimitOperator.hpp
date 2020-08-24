#ifndef LIMITOPERATOR_HPP
#define LIMITOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename T >
    class LimitOperator: public Operator {
    public:
        LimitOperator(int n): n(n), nb(0)
        {}

        ~LimitOperator()
        {}

        void* svc(void* task) {
            if (nb < n) {
                nb += 1;
                return (T*) task;
            } else {
                return (T*) GO_ON;
            }
        }
        
    private:
        int n;
        int nb;
    };

}

#endif
