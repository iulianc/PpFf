#ifndef NODE_HPP
#define NODE_HPP

#include <ff/ff.hpp>

using namespace ff;

namespace PpFf {

    enum NodeType { BASIC_NODE, WORKER_NODE, FARM_NODE, PIPELINE_NODE, UNDEFINED_NODE };

    class Node {
    public:
        virtual NodeType type(){
            return nodeType;
        }

        void addNode(ff_node *ffNode) {
            _ff_node = ffNode;
        }

        virtual ff_node* ff_node_() {
            return _ff_node;
        }

    protected:
        NodeType nodeType = BASIC_NODE;

    private:
        ff_node *_ff_node;
    };

}

#endif


