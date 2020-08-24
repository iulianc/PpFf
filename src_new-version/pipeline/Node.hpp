#ifndef NODE_HPP
#define NODE_HPP

#include <ff/ff.hpp>
#include <operators/Empty.hpp>
#include <operators/Operator.hpp>


namespace PpFf {

    class Node {
    public:
        Node(Operator* node = NULL) : _ff_node(node)
        {}

        virtual ~Node() {
            delete _ff_node;
        } 

        virtual ff::ff_node* build_ff_node() {
            return _ff_node;
        }

        bool isSource() {
            return _ff_node->isSource();
        }

        bool isCollector() {
            return _ff_node->isCollector();
        }        

    private:
        Operator* _ff_node; 
    };
}

#endif


