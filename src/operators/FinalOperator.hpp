#ifndef FINALOPERATOR_HPP
#define FINALOPERATOR_HPP

#include <operators/BaseOperator.hpp>

namespace PpFf {

    class FinalOperator: public BaseOperator {
    protected:
        bool const isFinalOperator = true;
        virtual void* svc(void *task) = 0;
    };

}

#endif
