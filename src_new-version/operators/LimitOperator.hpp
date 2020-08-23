#ifndef LIMITOPERATOR_HPP
#define LIMITOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>

namespace PpFf {

    template < typename T >
    class LimitOperator: public ff::ff_node {
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
