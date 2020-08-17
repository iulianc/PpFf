#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <ff/ff.hpp>
#include <pipeline/Node.hpp>

using namespace ff;

namespace PpFf {

    class Pipeline: public Node {
    public:
        Pipeline() {
            nodeType = PIPELINE_NODE;
            pipe = new ff_pipeline();
        }
        
        NodeType type(){
            return nodeType;
        }

        void addStage(ff_node *ffNode) {
            pipe->add_stage(ffNode);
        }

        void addStage(Node stage) {
            pipe->add_stage(stage.getNode());
        }

        ff_node* getNode() {
            return pipe;
        }

        void run() {
            if (pipe->run_and_wait_end() < 0) error("running pipeline");
        }

    private:
        ff_pipeline *pipe;
    };

}

#endif
