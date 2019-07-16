#ifndef NODE_HPP
#define NODE_HPP

//#include <ff/pipeline.hpp>
//#include <ff/farm.hpp>
#include <ff/ff.hpp>

using namespace ff;

namespace PpFf {

    enum NodeTypes { NodeTypeNode, NodeTypeWorker, NodeTypeFarm , NodeTypePipeline, Undefined };

    class Node {
    public:
        virtual NodeTypes Type(){
            return type;
        }

        void addStage(ff_node *ffNode) {
            node = ffNode;
        }

        virtual ff_node* getNode() {
            return node;
        }

    protected:
        NodeTypes type = NodeTypeNode;

    private:
        ff_node *node;
    };

}

#endif


