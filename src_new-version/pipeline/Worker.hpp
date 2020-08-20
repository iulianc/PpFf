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

        ~Worker() {
            for (unsigned int i = 0; i < nodes.size(); i++) {
                delete nodes[i];
            }

            nodes.clear();
        }          

        void* svc(void* task) {
            return NULL;
        }

        void addNode(Node* node) {
            nodes.push_back(node);
        }        

        ff_node* ff_node_() {
            if (nodes.size() ==  1) {
                return nodes[0];
            } else {
                ff_pipeline *pipe = new ff_pipeline();
                for (ff_node* node: nodes) {
                    pipe->add_stage(node);
                }
                return pipe;
            }
        }

    private:
        std::vector<Node*> nodes;
    };

}

#endif

