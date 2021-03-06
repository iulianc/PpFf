#ifndef WORKER_HPP
#define WORKER_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>

using namespace ff;

namespace PpFf {

    class Worker: public Node {
    public:
        Worker() {
            nodeType = WORKER_NODE;
        }

        NodeType type(){
            return nodeType;
        }

        void addNode(ff_node *ffNode) {
            nodes.push_back(ffNode);
        }

        ff_node* ff_node_() {
            if (nodes.size() > 1) {
                ff_pipeline *pipe = new ff_pipeline();
                for (ff_node* node: nodes) {
                    pipe->add_stage(node);
                }
                return pipe;
            } else {
                return nodes[0];
            }
        }

    private:
        std::vector<ff_node*> nodes;
    };

}

#endif

