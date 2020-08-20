#ifndef NODE_HPP
#define NODE_HPP

#include <ff/ff.hpp>

using namespace ff;

namespace PpFf {

    enum NodeType { BASIC_NODE, WORKER_NODE, FARM_NODE, PIPELINE_NODE, UNDEFINED_NODE };

    class Node : public ff_node {
    public:
        virtual NodeType type(){
            return nodeType;
        }

        virtual void* svc(void* task) = 0;

        virtual ff_node* ff_node_() {
            return this;
        }

    protected:
        NodeType nodeType = BASIC_NODE;
    };

}

#endif


