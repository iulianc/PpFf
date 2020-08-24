#ifndef FLATOPERATOR_HPP
#define FLATOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>
#include <operators/Operator.hpp>

namespace PpFf {

    template < typename TContainer, typename Out >
    class FlatOperator: public Operator {
    public:
        FlatOperator()
        {}

        ~FlatOperator()
        {}

        void* svc(void* task) {
            for (auto &elem : *(TContainer*)task) {
                this->ff_send_out(&elem);
            }

            return GO_ON;
        }
    };

}

#endif
