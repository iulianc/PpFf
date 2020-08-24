#ifndef EMPTY_HPP
#define EMPTY_HPP

#include <ff/ff.hpp>
#include <operators/Operator.hpp>

namespace PpFf {

    class Empty: public Operator {
    public:
        void* svc(void* task) {
            return task;
        }
    };

}

#endif
