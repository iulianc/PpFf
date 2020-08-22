#ifndef WORKER_HPP
#define WORKER_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>

using namespace ff;

namespace PpFf {

    class Worker: public Node {
    public:
        Worker() {
        }

        ~Worker() {
            for (unsigned int i = 0; i < nodes.size(); i++) {
                delete nodes[i];
            }

            nodes.clear();
        }          

        void addNode(Node* node) {
            nodes.push_back(node);
        }        

        ff_node* build_ff_node() {
            if (nodes.size() ==  1) {
                return nodes[0]->build_ff_node();
            } else {
                ff_pipeline *pipe = new ff_pipeline();
                for (Node* node: nodes) {
                    pipe->add_stage(node->build_ff_node());
                }
                return pipe;
            }
        }

    private:
        std::vector<Node*> nodes;
    };

}

#endif

