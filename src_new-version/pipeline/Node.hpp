#ifndef NODE_HPP
#define NODE_HPP

#include <ff/ff.hpp>
#include <operators/Empty.hpp>


namespace PpFf {

    class Node {
    public:
        Node(ff::ff_node* node = NULL) : _ff_node(node)
        {}

        virtual ~Node() {
            delete _ff_node;
        } 

        virtual ff::ff_node* build_ff_node() {
            return _ff_node;
        }

    private:
        ff::ff_node* _ff_node; 
    };
}

#endif


