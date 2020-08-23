#ifndef SKIPOPERATOR_HPP
#define SKIPOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>


namespace PpFf {

    template < typename T >
    class SkipOperator: public ff::ff_node {
    public:
        SkipOperator(int n): n(n), nb(0)
        {}

        ~SkipOperator()
        {}

        void* svc(void* task) {
            if (nb >= n) {
                return (T*) task;
            } else {
                nb += 1;
                return (T*) GO_ON;
            }
        }

    private:
        int n;
        int nb;
    };

}

#endif
