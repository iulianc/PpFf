#ifndef FLATOPERATOR_HPP
#define FLATOPERATOR_HPP

#include <ff/ff.hpp>
#include <functional>

namespace PpFf {

    template < typename TContainer, typename Out >
    class FlatOperator: public ff::ff_node {
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
