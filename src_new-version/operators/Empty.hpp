#ifndef EMPTY_HPP
#define EMPTY_HPP

#include <ff/ff.hpp>

namespace PpFf {

    class Empty: public ff::ff_node {
    public:
        void* svc(void* task) {
            return task;
        }
    };

}

#endif
