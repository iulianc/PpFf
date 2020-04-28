#ifndef LIMITOPERATOR_HPP
#define LIMITOPERATOR_HPP

#include <operators/BaseOperator.hpp>
#include <functional>


namespace PpFf {

    template < typename T >
    class LimitOperator: public BaseOperator {
    public:
        LimitOperator(int n): n(n), nb(0) {};

        ~LimitOperator() {}

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
