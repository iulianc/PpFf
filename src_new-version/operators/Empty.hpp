#ifndef EMPTY_HPP
#define EMPTY_HPP

#include <ff/ff.hpp>
#include <operators/IntermediateOp.hpp>

namespace PpFf {

    class Empty: public IntermediateOp {
    public:
        void* svc(void* task) {
            return task;
        }
    };

}

#endif
