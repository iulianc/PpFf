#ifndef NODE_HPP
#define NODE_HPP

#include <ff/ff.hpp>

using namespace ff;

namespace PpFf {

    enum NodeType { BASIC_NODE, WORKER_NODE, FARM_NODE };

    class Node : public ff_node {
    public:
        NodeType type() {
            return nodeType;
        }

        virtual void* svc(void* task) = 0;

        virtual ff_node* build_ff_node() {
            return this;
        }

    protected:
        NodeType nodeType = BASIC_NODE;
    };

}

#endif


