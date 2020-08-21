#ifndef NODE_HPP
#define NODE_HPP

#include <ff/ff.hpp>

using namespace ff;

namespace PpFf {

    class Node : public ff_node {
    public:
        virtual void* svc(void* task) = 0;
        
        virtual ff_node* build_ff_node() {
            return this;
        }
    };

}

#endif


