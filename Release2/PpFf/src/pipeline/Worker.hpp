#ifndef WORKER_HPP
#define WORKER_HPP

#include <ff/pipeline.hpp>
#include <pipeline/Node.hpp>

using namespace ff;

namespace PpFf {

    class Worker: public Node {
    public:
        Worker() {
            type = NodeTypeWorker;
        }

        NodeTypes Type(){
            return type;
        }

        void addStage(ff_node *ffNode) {
            nodes.push_back(ffNode);
        }

        ff_node* getNode() {
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

